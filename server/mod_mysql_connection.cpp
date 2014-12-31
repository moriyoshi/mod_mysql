/*
 * mod_mysql
 *
 * Copyright (c) 2007 Moriyoshi Koizumi. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <httpd.h>

#undef PACKAGE_NAME
#undef PACKAGE_VERSION
#undef PACKAGE_TARNAME
#undef PACKAGE_STRING
#undef PACKAGE_BUGREPORT

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <apr_strings.h>
#include <http_core.h>
#include <unistd.h>

#include "common.h"
#include "mod_mysql.h"
#include "mod_mysql_connection.h"
#include "mod_mysql_command.h"
#include "mod_mysql_command_handler.h"
#include "mod_mysql_authenticator.h"
#include "mod_mysql_exception.h"
#include "mod_mysql_illegal_state_exception.h"
#include "mod_mysql_recoverable_exception.h"
#include "mod_mysql_error.h"
#include "mod_mysql_field_info.h"
#include "mod_mysql_client_info.h"
#include "mod_mysql_command.h"
#include "mod_mysql_result.h"
#include "mod_mysql_fields.h"
#include "mod_mysql_service.h"
#include "mod_mysql_command_handler.h"
#include "mod_mysql_data_buffer.h"
#include "mod_mysql_server_message.h"
#include "mod_mysql_locale_registry.h"
#include "safety.h"

// {{{ mod_mysql_connection::mod_mysql_connection()
mod_mysql_connection::mod_mysql_connection(conn_rec* _conn,
        mod_mysql_config* _cfg, mod_mysql_logger* _log,
        mod_mysql_authenticator* _auth)
        : conn(_conn), cfg(_cfg), log(_log), cmd_hdlr(*this),
          pool(mod_mysql_pool::wrap(_conn->pool)), auth(_auth),
          in(*pool, _conn->bucket_alloc, _conn->input_filters),
          out(*pool, _conn->bucket_alloc, _conn->output_filters),
          bf(_conn->bucket_alloc), packet_seq(0), cinfo(0)
{
}
// }}}

// {{{ mod_mysql_connection::pack_and_flush()
void mod_mysql_connection::pack_and_flush()
{
    apr_size_t len = out.get_length(false);
    log->debug(APLOG_MARK, 0, "sending %d bytes", len + 4);
    apr_byte_t pkt_hdr[4] = { len, len >> 8, len >> 16, packet_seq++ };
    out.prepend(
            bf.create_transient_bucket(
                reinterpret_cast<const char*>(pkt_hdr),
                sizeof(pkt_hdr)));
    out.append(bf.create_flush_bucket());
    out.pass();
}
// }}}

// {{{ mod_mysql_connection::wait_for_packet()
apr_size_t mod_mysql_connection::wait_for_packet()
{
    apr_byte_t pkt_hdr[4];

    apr_size_t nbytesread = read(pkt_hdr, sizeof(pkt_hdr));
    if (nbytesread == 0) {
        conn->aborted = true;
        throw mod_mysql_exception("Disconnected forcefully");
    } else if (nbytesread < sizeof(pkt_hdr)) {
        throw mod_mysql_exception("Unexpected end of stream");
    }

    apr_size_t pkt_size = static_cast<apr_size_t>(pkt_hdr[0])
            + (static_cast<apr_size_t>(pkt_hdr[1]) << 8)
            + (static_cast<apr_size_t>(pkt_hdr[2]) << 16);

    log->debug(APLOG_MARK, 0, "received packet size=%d, seq=%u (expecting %u)",
            pkt_size + 4, pkt_hdr[3], packet_seq);

    if (pkt_hdr[3] != packet_seq) {
        throw mod_mysql_recoverable_exception(
                MOD_MYSQL_MSG_NET_PACKETS_OUT_OF_ORDER,
                "skewed packet order");
    }

    packet_seq++;

    return pkt_size;
}
// }}}

// {{{ mod_mysql_connection::dump()
void mod_mysql_connection::dump(const apr_byte_t* buf, apr_size_t sz)
{
    static const int ncolumns = 16;

    const apr_byte_t* p = buf;
    const apr_byte_t* lim = buf + sz;

    fprintf(stderr, "Hexadecimal dump of %u bytes:\n", sz);
    while (p < lim) {
        for (int i = 0; i < ncolumns; i++, p++) {
            if (i > 0) {
                fprintf(stderr, " ");
            }
            if (p < lim) {
                fprintf(stderr, "%02x", *p);
            } else {
                fprintf(stderr, "  ");
            }
        }
        fprintf(stderr, "|");
        p -= ncolumns;
        for (int i = 0; i < ncolumns; i++, p++) {
            if (p < lim && *p >= 0x20 && *p <= 0x7e) {
                fprintf(stderr, "%c", *p);
            } else {
                fprintf(stderr, ".");
            }
        }
        fprintf(stderr, "\n");
    }
}
// }}}

// {{{ mod_mysql_connection::reply()
void mod_mysql_connection::reply(const mod_mysql_server_message& msg)
{
    out.write(0xff);
    out.writele((unsigned short)msg.get_code());
    if (msg.get_message())
        out.write(msg.get_message());
    pack_and_flush();
    out.pass();
}

void mod_mysql_connection::reply(const mod_mysql_result& result)
{
    mod_mysql_fields* fields = result.get_fields();

    if (!fields || fields->count() == 0) {
        out.write((apr_byte_t)0);
        out.write((apr_byte_t)result.get_num_affected_rows());
        out.write((apr_byte_t)result.get_last_insert_id());
        out.writele((unsigned short)result.get_code());
        out.writele((unsigned short)result.get_num_warnings());
        pack_and_flush();
        out.pass();
        return;
    }

    out.write((apr_byte_t)fields->count());
    pack_and_flush();
    out.pass();

    {
        mod_mysql_pool my_pool(pool);

        if (cinfo->capabilities & mod_mysql_capabilities::PROTOCOL_41) {
            for (mod_mysql_iterator<mod_mysql_field_info*>* i =
                    fields->create_iterator(&my_pool);
                    i->valid(); i->next()) {
                mod_mysql_field_info* const f = i->current();
                mod_mysql_locale* const loc = f->get_locale();

                out.write_nts(f->get_catalogue_name());
                out.write_nts(f->get_database_name());
                out.write_nts(f->get_table_name());
                out.write_nts(f->get_original_table_name());
                out.write_nts(f->get_name());
                out.write_nts(f->get_original_name());

                apr_byte_t buf[12];
                {
                    mod_mysql_data_buffer b(
                            mod_mysql_ranged_ref<apr_byte_t>(buf, sizeof(buf)));
                    b.writele((apr_uint16_t)(loc ? loc->get_code(): 8));
                    b.writele((apr_uint32_t)f->get_length());
                    b.write((apr_byte_t)f->get_type());
                    b.writele((apr_uint16_t)f->get_attributes());
                    b.write((apr_byte_t)f->get_num_decimals());
                }
                out.write((apr_byte_t)sizeof(buf));
                out.write(reinterpret_cast<char*>(buf), sizeof(buf));
            }
        } else {
            if (cinfo->capabilities & mod_mysql_capabilities::LONG_FLAG) {
                for (mod_mysql_iterator<mod_mysql_field_info*>* i =
                        fields->create_iterator(&my_pool);
                        i->valid(); i->next()) {
                    mod_mysql_field_info* f = i->current();
                    apr_byte_t buf[10];

                    buf[0] = 3;
                    {
                        unsigned long len = f->get_length();
                        buf[1] = len        & 0xff;
                        buf[2] = (len >> 1) & 0xff;
                        buf[3] = (len >> 2) & 0xff;
                    }
                    buf[4] = 1;
                    buf[5] = f->get_type();
                    buf[6] = 3;
                    {
                        unsigned int attr = f->get_attributes();
                        buf[7] = attr        & 0xff;
                        buf[8] = (attr >> 8) & 0xff;
                    }
                    buf[9] = f->get_num_decimals();
                    out.write(reinterpret_cast<char *>(buf), sizeof(buf));
                }
            } else {
                for (mod_mysql_iterator<mod_mysql_field_info*>* i =
                        fields->create_iterator(&my_pool);
                        i->valid(); i->next()) {
                    mod_mysql_field_info* f = i->current();
                    apr_byte_t buf[9];

                    buf[0] = 3;
                    {
                        unsigned long len = f->get_length();
                        buf[1] = len        & 0xff;
                        buf[2] = (len >> 1) & 0xff;
                        buf[3] = (len >> 2) & 0xff;
                    }
                    buf[4] = 1;
                    buf[5] = f->get_type();
                    buf[6] = 2;
                    buf[7] = f->get_attributes();
                    buf[8] = f->get_num_decimals();
                    out.write(reinterpret_cast<char *>(buf), sizeof(buf));
                }
            }
        }
    }
}
// }}}

// {{{ mod_mysql_connection::read()
apr_size_t mod_mysql_connection::read(apr_byte_t* buf, apr_size_t buf_sz)
{
    if (in.get_length() < buf_sz) {
        in.fill(AP_MODE_READBYTES, APR_BLOCK_READ, buf_sz - in.get_length());
    }

    apr_byte_t* p = buf;

    for (mod_mysql_bucket_brigade::iterator i = in.create_iterator();
            buf_sz > 0 && i.valid(); i.next()) {
        const char *tmp_buf;
        apr_size_t tmp_buf_sz;
        mod_mysql_bucket* b = i.current();

        b->read(&tmp_buf, &tmp_buf_sz, APR_BLOCK_READ);
        if (buf_sz >= tmp_buf_sz) {
            memcpy(p, tmp_buf, tmp_buf_sz);
            buf_sz -= tmp_buf_sz;
            p += tmp_buf_sz;
        } else {
            memcpy(p, tmp_buf, buf_sz);
            b->split(buf_sz);
            buf_sz = 0;
            p += buf_sz;
        }

        i.remove();
    }

    return p - buf;
}
// }}}

// {{{ mod_mysql_connection::waste()
void mod_mysql_connection::waste(apr_size_t sz)
{
    if (in.get_length() < sz) {
        in.fill(AP_MODE_READBYTES, APR_BLOCK_READ, sz - in.get_length());
    }

    for (mod_mysql_bucket_brigade::iterator i = in.create_iterator();
            sz > 0 && i.valid(); i.next()) {
        mod_mysql_bucket* b = i.current();
        const char *tmp_buf;
        apr_size_t tmp_buf_sz;

        b->read(&tmp_buf, &tmp_buf_sz, APR_BLOCK_READ);
        if (sz >= tmp_buf_sz) {
            sz -= tmp_buf_sz;
        } else {
            b->split(sz);
            sz = 0;
        }

        in.remove(b);
    }
}
// }}}

// {{{ mod_mysql_connection::do_authenticate()
mod_mysql_server_message* mod_mysql_connection::do_authenticate(
        mod_mysql_pool* pool,
        const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash)
{
    return auth->authenticate(pool, cinfo,
            mod_mysql_ranged_ref<const apr_byte_t>(pw_seed,
                    cinfo->capabilities & mod_mysql_capabilities::PROTOCOL_41 ?
                            mod_mysql::pw_seed_len_41:
                            mod_mysql::pw_seed_len_323), pw_hash);
}
// }}}

// {{{ mod_mysql_connection::do_expect_client_greet()
bool mod_mysql_connection::do_expect_client_greet()
{
    apr_size_t pkt_sz = wait_for_packet();
    if (pkt_sz > MAX_USER_NAME_LENGTH + 1 + 54) {
        waste(pkt_sz);
        throw mod_mysql_exception(apr_psprintf(*pool,
                "Wrong greeting from the client (got %d bytes)", pkt_sz));
    }

    mod_mysql_pool my_pool(pool);
    apr_byte_t* buf = static_cast<apr_byte_t*>(my_pool.alloc(pkt_sz));
    apr_size_t nbytesread;

    if ((nbytesread = read(buf, pkt_sz)) < pkt_sz) {
        throw mod_mysql_exception("Unexpected end of stream");
    }

    apr_byte_t* p = buf;
    apr_byte_t* lim = buf + nbytesread;

    unsigned long mapkt = 0x10000000;
    mod_mysql_locale* loc = 0;
    const apr_byte_t* user = 0;
    mod_mysql_memblock<apr_byte_t>* pw_hash;

    mod_mysql_capabilities caps = static_cast<unsigned long>(p[0])
            | (static_cast<unsigned long>(p[1]) << 8);
    p += 2;

    if (caps & mod_mysql_capabilities::PROTOCOL_41) {
        if (p >= lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        }

        caps |= (static_cast<unsigned long>(p[2]) << 16)
            | (static_cast<unsigned long>(p[3]) << 24);
        p += 2;

        if (p >= lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        }
        mapkt = static_cast<unsigned long>(p[0])
            | (static_cast<unsigned long>(p[1]) << 8)
            | (static_cast<unsigned long>(p[2]) << 16)
            | (static_cast<unsigned long>(p[3]) << 24);
        p += 4;
        if (p >= lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        }
        mod_mysql_locale::code_e loc_c =
                static_cast<mod_mysql_locale::code_e>(
                    static_cast<unsigned int>(p[0]));
        loc = mod_mysql::get_locale_registry()->find(loc_c);
        if (!loc) {
            throw mod_mysql_exception(apr_psprintf(*pool,
                    "Unknown locale: %d", loc_c));
        }
        p += 24;

        if (p > lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        }
    } else {
        mapkt = static_cast<unsigned long>(p[0])
            | (static_cast<unsigned long>(p[1]) << 8)
            | (static_cast<unsigned long>(p[2]) << 16);
        p += 3;
        if (p > lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        }
    }

    if (p < lim) {
        size_t len = strlen(reinterpret_cast<char*>(p));
        user = pool->dup<apr_byte_t>(p, len);

        p += len + 1;

        if (p > lim) {
            throw mod_mysql_exception("Unexpected end of packet");
        } else if (p < lim) {
            apr_size_t pw_hash_len;
            if (caps & mod_mysql_capabilities::SECURE_CONNECTION) {
                pw_hash_len = p[0];
                p++;
            } else {
                pw_hash_len = strlen(reinterpret_cast<char*>(p));
            }

            if (lim - p < pw_hash_len) {
                throw mod_mysql_exception("Unexpected end of packet");
            }

            pw_hash = mod_mysql_memblock<apr_byte_t>::create(
                    &my_pool, pw_hash_len);
            memcpy(*pw_hash, p, pw_hash_len);
        }
    }

    log->debug(APLOG_MARK, 0, "capability=%d", (int)caps);
    log->debug(APLOG_MARK, 0, "locale=%s", loc ?
            loc->get_name(): "(unknown)");
    log->debug(APLOG_MARK, 0, "user=%s", user);

    cinfo = new(pool) mod_mysql_client_info(
            caps, loc, mapkt, user,
            reinterpret_cast<apr_byte_t*>(conn->remote_ip),
            reinterpret_cast<apr_byte_t*>(conn->remote_host),
            reinterpret_cast<apr_byte_t*>(conn->remote_logname));

    mod_mysql_server_message* msg = do_authenticate(&my_pool,
            reinterpret_cast<const mod_mysql_ranged_ref<const apr_byte_t>&>(
                *pw_hash));

    if (msg) {
        reply(*msg);
        log->debug(APLOG_MARK, 0, "Authentication failure");
    } else {
        reply(mod_mysql_result(pool, mod_mysql_result::code_t::AUTOCOMMIT));
        log->debug(APLOG_MARK, 0, "Authentication success");
    }

    packet_seq = 0;

    return !msg ? true: false;
}
// }}}

// {{{ mod_mysql_connection::do_greet()
bool mod_mysql_connection::do_greet()
{
    apr_generate_random_bytes(pw_seed, sizeof(pw_seed));

    // protocol version
    out.write(0x0a);
    // version string
    out.write(PACKAGE_STRING, sizeof(PACKAGE_STRING));

    apr_byte_t buf[128];
    mod_mysql_data_buffer b(mod_mysql_ranged_ref<apr_byte_t>(buf, sizeof(buf)));
    // thread id
    b.writele((apr_uint32_t)conn->id);
    // the salt
    b.write(mod_mysql_ranged_ref<const apr_byte_t>(pw_seed, 9));
    // capabilities
    b.writele((apr_uint16_t)(
            mod_mysql_capabilities::PROTOCOL_41
            | mod_mysql_capabilities::SECURE_CONNECTION));
    // locale 
    b.writele((apr_uint16_t)
            (*mod_mysql::get_locale_registry())[
                MOD_MYSQL_LOC_UTF8_GENERAL_CI]->get_code());
    // status
    b.writele((apr_uint16_t)mod_mysql_result::code_t::AUTOCOMMIT);
    b.pad(0, 13);
    // rest of the salt
    b.write(mod_mysql_ranged_ref<const apr_byte_t>(pw_seed + 8, 13));

    out.write((mod_mysql_ranged_ref<const apr_byte_t>)b);

    pack_and_flush();
    out.pass();

    return true;
}
// }}}

// {{{ mod_mysql_connection::do_initial_handshake()
bool mod_mysql_connection::do_initial_handshake()
{
    return do_greet() && do_expect_client_greet();
}
// }}}

// {{{ mod_mysql_connection::do_handle_packet()
void mod_mysql_connection::do_handle_packet(
        mod_mysql_pool* my_pool, const apr_byte_t* pkt, apr_size_t pkt_sz)
{
    if (pkt_sz < 1) {
        throw mod_mysql_recoverable_exception(
                MOD_MYSQL_MSG_NET_READ_ERROR, "Packet too short");
    }

    mod_mysql_command cmd = (mod_mysql_command)pkt[0];

    switch (cmd) {
    case MOD_MYSQL_COMMAND_SLEEP:
        reply(*((mod_mysql_command_handler *)cmd_hdlr)->handle_sleep(my_pool));
        break;
    case MOD_MYSQL_COMMAND_QUIT:
        reply(*((mod_mysql_command_handler *)cmd_hdlr)->handle_quit(my_pool));
        break;
    case MOD_MYSQL_COMMAND_INIT_DB:
        break;
    case MOD_MYSQL_COMMAND_QUERY:
    case MOD_MYSQL_COMMAND_FIELD_LIST:
    case MOD_MYSQL_COMMAND_CREATE_DB:
    case MOD_MYSQL_COMMAND_DROP_DB:
    case MOD_MYSQL_COMMAND_REFRESH:
    case MOD_MYSQL_COMMAND_SHUTDOWN:
    case MOD_MYSQL_COMMAND_STATISTICS:
    case MOD_MYSQL_COMMAND_PROCESS_INFO:
    case MOD_MYSQL_COMMAND_CONNECT:
    case MOD_MYSQL_COMMAND_PROCESS_KILL:
    case MOD_MYSQL_COMMAND_DEBUG:
    case MOD_MYSQL_COMMAND_PING:
    case MOD_MYSQL_COMMAND_TIME:
    case MOD_MYSQL_COMMAND_DELAYED_INSERT:
    case MOD_MYSQL_COMMAND_CHANGE_USER:
    case MOD_MYSQL_COMMAND_BINLOG_DUMP:
    case MOD_MYSQL_COMMAND_TABLE_DUMP:
    case MOD_MYSQL_COMMAND_CONNECT_OUT:
    case MOD_MYSQL_COMMAND_REGISTER_SLAVE:
    case MOD_MYSQL_COMMAND_PREPARE:
    case MOD_MYSQL_COMMAND_EXECUTE:
    case MOD_MYSQL_COMMAND_LONG_DATA:
    case MOD_MYSQL_COMMAND_CLOSE_STMT:
    case MOD_MYSQL_COMMAND_RESET_STMT:
    case MOD_MYSQL_COMMAND_SET_OPTION:
    case MOD_MYSQL_COMMAND_FETCH:
    case MOD_MYSQL_COMMAND_UPLOAD_FILE:
    case MOD_MYSQL_COMMAND_MESSAGE:
        break;
    }

    reply(mod_mysql_result(my_pool, mod_mysql_result::code_t::AUTOCOMMIT));
    packet_seq = 0;
}
// }}}

// {{{ mod_mysql_connection::process()
int mod_mysql_connection::process()
{
    try {
        if (!do_initial_handshake()) {
            return OK;
        }

        for (;;) {
            mod_mysql_pool my_pool(pool);

            try {
                apr_size_t pkt_sz = wait_for_packet();
                if (pkt_sz > cfg->get_max_allowed_packet()) {
                    waste(pkt_sz);
                    throw mod_mysql_recoverable_exception(
                            MOD_MYSQL_MSG_NET_PACKET_TOO_LARGE,
                            "Too large packet");
                }

                apr_byte_t* pkt = reinterpret_cast<apr_byte_t*>(
                        my_pool.alloc(pkt_sz));
                read(pkt, pkt_sz);

                do_handle_packet(&my_pool, pkt, pkt_sz);
            } catch (mod_mysql_recoverable_exception e) {
                reply(mod_mysql_server_message(e.get_code(),
                        e.get_message()));
                packet_seq = 0;
            }
        }
    } catch (mod_mysql_exception e) {
        log->notice(APLOG_MARK, 0, e.get_message());
    } catch (mod_mysql_error e) {
        log->log(mod_mysql_logger::ERR, e);
    }

    delete cmd_hdlr;

    return OK;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
