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
#ifndef _MOD_MYSQL_SERVER_MESSAGE_H
#define _MOD_MYSQL_SERVER_MESSAGE_H

enum mod_mysql_server_message_code_t
{
    MOD_MYSQL_MSG_NONE                                    =    0,
    MOD_MYSQL_MSG_HASHCHK                                 = 1000,
    MOD_MYSQL_MSG_NISAMCHK                                = 1001,
    MOD_MYSQL_MSG_NO                                      = 1002,
    MOD_MYSQL_MSG_YES                                     = 1003,
    MOD_MYSQL_MSG_CANT_CREATE_FILE                        = 1004,
    MOD_MYSQL_MSG_CANT_CREATE_TABLE                       = 1005,
    MOD_MYSQL_MSG_CANT_CREATE_DB                          = 1006,
    MOD_MYSQL_MSG_DB_CREATE_EXISTS                        = 1007,
    MOD_MYSQL_MSG_DB_DROP_EXISTS                          = 1008,
    MOD_MYSQL_MSG_DB_DROP_DELETE                          = 1009,
    MOD_MYSQL_MSG_DB_DROP_RMDIR                           = 1010,
    MOD_MYSQL_MSG_CANT_DELETE_FILE                        = 1011,
    MOD_MYSQL_MSG_CANT_FIND_SYSTEM_REC                    = 1012,
    MOD_MYSQL_MSG_CANT_GET_STAT                           = 1013,
    MOD_MYSQL_MSG_CANT_GET_WD                             = 1014,
    MOD_MYSQL_MSG_CANT_LOCK                               = 1015,
    MOD_MYSQL_MSG_CANT_OPEN_FILE                          = 1016,
    MOD_MYSQL_MSG_FILE_NOT_FOUND                          = 1017,
    MOD_MYSQL_MSG_CANT_READ_DIR                           = 1018,
    MOD_MYSQL_MSG_CANT_SET_WD                             = 1019,
    MOD_MYSQL_MSG_CHECKREAD                               = 1020,
    MOD_MYSQL_MSG_DISK_FULL                               = 1021,
    MOD_MYSQL_MSG_DUP_KEY                                 = 1022,
    MOD_MYSQL_MSG_ERROR_ON_CLOSE                          = 1023,
    MOD_MYSQL_MSG_ERROR_ON_READ                           = 1024,
    MOD_MYSQL_MSG_ERROR_ON_RENAME                         = 1025,
    MOD_MYSQL_MSG_ERROR_ON_WRITE                          = 1026,
    MOD_MYSQL_MSG_FILE_USED                               = 1027,
    MOD_MYSQL_MSG_FILSORT_ABORT                           = 1028,
    MOD_MYSQL_MSG_FORM_NOT_FOUND                          = 1029,
    MOD_MYSQL_MSG_GET_ERRNO                               = 1030,
    MOD_MYSQL_MSG_ILLEGAL_HA                              = 1031,
    MOD_MYSQL_MSG_KEY_NOT_FOUND                           = 1032,
    MOD_MYSQL_MSG_NOT_FORM_FILE                           = 1033,
    MOD_MYSQL_MSG_NOT_KEYFILE                             = 1034,
    MOD_MYSQL_MSG_OLD_KEYFILE                             = 1035,
    MOD_MYSQL_MSG_OPEN_AS_READONLY                        = 1036,
    MOD_MYSQL_MSG_OUTOFMEMORY                             = 1037,
    MOD_MYSQL_MSG_OUT_OF_SORTMEMORY                       = 1038,
    MOD_MYSQL_MSG_UNEXPECTED_EOF                          = 1039,
    MOD_MYSQL_MSG_CON_COUNT_ERROR                         = 1040,
    MOD_MYSQL_MSG_OUT_OF_RESOURCES                        = 1041,
    MOD_MYSQL_MSG_BAD_HOST_ERROR                          = 1042,
    MOD_MYSQL_MSG_HANDSHAKE_ERROR                         = 1043,
    MOD_MYSQL_MSG_DBACCESS_DENIED_ERROR                   = 1044,
    MOD_MYSQL_MSG_ACCESS_DENIED_ERROR                     = 1045,
    MOD_MYSQL_MSG_NO_DB_ERROR                             = 1046,
    MOD_MYSQL_MSG_UNKNOWN_COM_ERROR                       = 1047,
    MOD_MYSQL_MSG_BAD_NULL_ERROR                          = 1048,
    MOD_MYSQL_MSG_BAD_DB_ERROR                            = 1049,
    MOD_MYSQL_MSG_TABLE_EXISTS_ERROR                      = 1050,
    MOD_MYSQL_MSG_BAD_TABLE_ERROR                         = 1051,
    MOD_MYSQL_MSG_NON_UNIQ_ERROR                          = 1052,
    MOD_MYSQL_MSG_SERVER_SHUTDOWN                         = 1053,
    MOD_MYSQL_MSG_BAD_FIELD_ERROR                         = 1054,
    MOD_MYSQL_MSG_WRONG_FIELD_WITH_GROUP                  = 1055,
    MOD_MYSQL_MSG_WRONG_GROUP_FIELD                       = 1056,
    MOD_MYSQL_MSG_WRONG_SUM_SELECT                        = 1057,
    MOD_MYSQL_MSG_WRONG_VALUE_COUNT                       = 1058,
    MOD_MYSQL_MSG_TOO_LONG_IDENT                          = 1059,
    MOD_MYSQL_MSG_DUP_FIELDNAME                           = 1060,
    MOD_MYSQL_MSG_DUP_KEYNAME                             = 1061,
    MOD_MYSQL_MSG_DUP_ENTRY                               = 1062,
    MOD_MYSQL_MSG_WRONG_FIELD_SPEC                        = 1063,
    MOD_MYSQL_MSG_PARSE_ERROR                             = 1064,
    MOD_MYSQL_MSG_EMPTY_QUERY                             = 1065,
    MOD_MYSQL_MSG_NONUNIQ_TABLE                           = 1066,
    MOD_MYSQL_MSG_INVALID_DEFAULT                         = 1067,
    MOD_MYSQL_MSG_MULTIPLE_PRI_KEY                        = 1068,
    MOD_MYSQL_MSG_TOO_MANY_KEYS                           = 1069,
    MOD_MYSQL_MSG_TOO_MANY_KEY_PARTS                      = 1070,
    MOD_MYSQL_MSG_TOO_LONG_KEY                            = 1071,
    MOD_MYSQL_MSG_KEY_COLUMN_DOES_NOT_EXITS               = 1072,
    MOD_MYSQL_MSG_BLOB_USED_AS_KEY                        = 1073,
    MOD_MYSQL_MSG_TOO_BIG_FIELDLENGTH                     = 1074,
    MOD_MYSQL_MSG_WRONG_AUTO_KEY                          = 1075,
    MOD_MYSQL_MSG_READY                                   = 1076,
    MOD_MYSQL_MSG_NORMAL_SHUTDOWN                         = 1077,
    MOD_MYSQL_MSG_GOT_SIGNAL                              = 1078,
    MOD_MYSQL_MSG_SHUTDOWN_COMPLETE                       = 1079,
    MOD_MYSQL_MSG_FORCING_CLOSE                           = 1080,
    MOD_MYSQL_MSG_IPSOCK_ERROR                            = 1081,
    MOD_MYSQL_MSG_NO_SUCH_INDEX                           = 1082,
    MOD_MYSQL_MSG_WRONG_FIELD_TERMINATORS                 = 1083,
    MOD_MYSQL_MSG_BLOBS_AND_NO_TERMINATED                 = 1084,
    MOD_MYSQL_MSG_TEXTFILE_NOT_READABLE                   = 1085,
    MOD_MYSQL_MSG_FILE_EXISTS_ERROR                       = 1086,
    MOD_MYSQL_MSG_LOAD_INFO                               = 1087,
    MOD_MYSQL_MSG_ALTER_INFO                              = 1088,
    MOD_MYSQL_MSG_WRONG_SUB_KEY                           = 1089,
    MOD_MYSQL_MSG_CANT_REMOVE_ALL_FIELDS                  = 1090,
    MOD_MYSQL_MSG_CANT_DROP_FIELD_OR_KEY                  = 1091,
    MOD_MYSQL_MSG_INSERT_INFO                             = 1092,
    MOD_MYSQL_MSG_UPDATE_TABLE_USED                       = 1093,
    MOD_MYSQL_MSG_NO_SUCH_THREAD                          = 1094,
    MOD_MYSQL_MSG_KILL_DENIED_ERROR                       = 1095,
    MOD_MYSQL_MSG_NO_TABLES_USED                          = 1096,
    MOD_MYSQL_MSG_TOO_BIG_SET                             = 1097,
    MOD_MYSQL_MSG_NO_UNIQUE_LOGFILE                       = 1098,
    MOD_MYSQL_MSG_TABLE_NOT_LOCKED_FOR_WRITE              = 1099,
    MOD_MYSQL_MSG_TABLE_NOT_LOCKED                        = 1100,
    MOD_MYSQL_MSG_BLOB_CANT_HAVE_DEFAULT                  = 1101,
    MOD_MYSQL_MSG_WRONG_DB_NAME                           = 1102,
    MOD_MYSQL_MSG_WRONG_TABLE_NAME                        = 1103,
    MOD_MYSQL_MSG_TOO_BIG_SELECT                          = 1104,
    MOD_MYSQL_MSG_UNKNOWN_ERROR                           = 1105,
    MOD_MYSQL_MSG_UNKNOWN_PROCEDURE                       = 1106,
    MOD_MYSQL_MSG_WRONG_PARAMCOUNT_TO_PROCEDURE           = 1107,
    MOD_MYSQL_MSG_WRONG_PARAMETERS_TO_PROCEDURE           = 1108,
    MOD_MYSQL_MSG_UNKNOWN_TABLE                           = 1109,
    MOD_MYSQL_MSG_FIELD_SPECIFIED_TWICE                   = 1110,
    MOD_MYSQL_MSG_INVALID_GROUP_FUNC_USE                  = 1111,
    MOD_MYSQL_MSG_UNSUPPORTED_EXTENSION                   = 1112,
    MOD_MYSQL_MSG_TABLE_MUST_HAVE_COLUMNS                 = 1113,
    MOD_MYSQL_MSG_RECORD_FILE_FULL                        = 1114,
    MOD_MYSQL_MSG_UNKNOWN_CHARACTER_SET                   = 1115,
    MOD_MYSQL_MSG_TOO_MANY_TABLES                         = 1116,
    MOD_MYSQL_MSG_TOO_MANY_FIELDS                         = 1117,
    MOD_MYSQL_MSG_TOO_BIG_ROWSIZE                         = 1118,
    MOD_MYSQL_MSG_STACK_OVERRUN                           = 1119,
    MOD_MYSQL_MSG_WRONG_OUTER_JOIN                        = 1120,
    MOD_MYSQL_MSG_NULL_COLUMN_IN_INDEX                    = 1121,
    MOD_MYSQL_MSG_CANT_FIND_UDF                           = 1122,
    MOD_MYSQL_MSG_CANT_INITIALIZE_UDF                     = 1123,
    MOD_MYSQL_MSG_UDF_NO_PATHS                            = 1124,
    MOD_MYSQL_MSG_UDF_EXISTS                              = 1125,
    MOD_MYSQL_MSG_CANT_OPEN_LIBRARY                       = 1126,
    MOD_MYSQL_MSG_CANT_FIND_DL_ENTRY                      = 1127,
    MOD_MYSQL_MSG_FUNCTION_NOT_DEFINED                    = 1128,
    MOD_MYSQL_MSG_HOST_IS_BLOCKED                         = 1129,
    MOD_MYSQL_MSG_HOST_NOT_PRIVILEGED                     = 1130,
    MOD_MYSQL_MSG_PASSWORD_ANONYMOUS_USER                 = 1131,
    MOD_MYSQL_MSG_PASSWORD_NOT_ALLOWED                    = 1132,
    MOD_MYSQL_MSG_PASSWORD_NO_MATCH                       = 1133,
    MOD_MYSQL_MSG_UPDATE_INFO                             = 1134,
    MOD_MYSQL_MSG_CANT_CREATE_THREAD                      = 1135,
    MOD_MYSQL_MSG_WRONG_VALUE_COUNT_ON_ROW                = 1136,
    MOD_MYSQL_MSG_CANT_REOPEN_TABLE                       = 1137,
    MOD_MYSQL_MSG_INVALID_USE_OF_NULL                     = 1138,
    MOD_MYSQL_MSG_REGEXP_ERROR                            = 1139,
    MOD_MYSQL_MSG_MIX_OF_GROUP_FUNC_AND_FIELDS            = 1140,
    MOD_MYSQL_MSG_NONEXISTING_GRANT                       = 1141,
    MOD_MYSQL_MSG_TABLEACCESS_DENIED_ERROR                = 1142,
    MOD_MYSQL_MSG_COLUMNACCESS_DENIED_ERROR               = 1143,
    MOD_MYSQL_MSG_ILLEGAL_GRANT_FOR_TABLE                 = 1144,
    MOD_MYSQL_MSG_GRANT_WRONG_HOST_OR_USER                = 1145,
    MOD_MYSQL_MSG_NO_SUCH_TABLE                           = 1146,
    MOD_MYSQL_MSG_NONEXISTING_TABLE_GRANT                 = 1147,
    MOD_MYSQL_MSG_NOT_ALLOWED_COMMAND                     = 1148,
    MOD_MYSQL_MSG_SYNTAX_ERROR                            = 1149,
    MOD_MYSQL_MSG_DELAYED_CANT_CHANGE_LOCK                = 1150,
    MOD_MYSQL_MSG_TOO_MANY_DELAYED_THREADS                = 1151,
    MOD_MYSQL_MSG_ABORTING_SERVER_MESSAGE                 = 1152,
    MOD_MYSQL_MSG_NET_PACKET_TOO_LARGE                    = 1153,
    MOD_MYSQL_MSG_NET_READ_ERROR_FROM_PIPE                = 1154,
    MOD_MYSQL_MSG_NET_FCNTL_ERROR                         = 1155,
    MOD_MYSQL_MSG_NET_PACKETS_OUT_OF_ORDER                = 1156,
    MOD_MYSQL_MSG_NET_UNCOMPRESS_ERROR                    = 1157,
    MOD_MYSQL_MSG_NET_READ_ERROR                          = 1158,
    MOD_MYSQL_MSG_NET_READ_INTERRUPTED                    = 1159,
    MOD_MYSQL_MSG_NET_ERROR_ON_WRITE                      = 1160,
    MOD_MYSQL_MSG_NET_WRITE_INTERRUPTED                   = 1161,
    MOD_MYSQL_MSG_TOO_LONG_STRING                         = 1162,
    MOD_MYSQL_MSG_TABLE_CANT_HANDLE_BLOB                  = 1163,
    MOD_MYSQL_MSG_TABLE_CANT_HANDLE_AUTO_INCREMENT        = 1164,
    MOD_MYSQL_MSG_DELAYED_INSERT_TABLE_LOCKED             = 1165,
    MOD_MYSQL_MSG_WRONG_COLUMN_NAME                       = 1166,
    MOD_MYSQL_MSG_WRONG_KEY_COLUMN                        = 1167,
    MOD_MYSQL_MSG_WRONG_MRG_TABLE                         = 1168,
    MOD_MYSQL_MSG_DUP_UNIQUE                              = 1169,
    MOD_MYSQL_MSG_BLOB_KEY_WITHOUT_LENGTH                 = 1170,
    MOD_MYSQL_MSG_PRIMARY_CANT_HAVE_NULL                  = 1171,
    MOD_MYSQL_MSG_TOO_MANY_ROWS                           = 1172,
    MOD_MYSQL_MSG_REQUIRES_PRIMARY_KEY                    = 1173,
    MOD_MYSQL_MSG_NO_RAID_COMPILED                        = 1174,
    MOD_MYSQL_MSG_UPDATE_WITHOUT_KEY_IN_SAFE_MODE         = 1175,
    MOD_MYSQL_MSG_KEY_DOES_NOT_EXITS                      = 1176,
    MOD_MYSQL_MSG_CHECK_NO_SUCH_TABLE                     = 1177,
    MOD_MYSQL_MSG_CHECK_NOT_IMPLEMENTED                   = 1178,
    MOD_MYSQL_MSG_CANT_DO_THIS_DURING_AN_TRANSACTION      = 1179,
    MOD_MYSQL_MSG_ERROR_DURING_COMMIT                     = 1180,
    MOD_MYSQL_MSG_ERROR_DURING_ROLLBACK                   = 1181,
    MOD_MYSQL_MSG_ERROR_DURING_FLUSH_LOGS                 = 1182,
    MOD_MYSQL_MSG_ERROR_DURING_CHECKPOINT                 = 1183,
    MOD_MYSQL_MSG_NEW_ABORTING_SERVER_MESSAGE             = 1184,
    MOD_MYSQL_MSG_DUMP_NOT_IMPLEMENTED                    = 1185,
    MOD_MYSQL_MSG_FLUSH_MASTER_BINLOG_CLOSED              = 1186,
    MOD_MYSQL_MSG_INDEX_REBUILD                           = 1187,
    MOD_MYSQL_MSG_MASTER                                  = 1188,
    MOD_MYSQL_MSG_MASTER_NET_READ                         = 1189,
    MOD_MYSQL_MSG_MASTER_NET_WRITE                        = 1190,
    MOD_MYSQL_MSG_FT_MATCHING_KEY_NOT_FOUND               = 1191,
    MOD_MYSQL_MSG_LOCK_OR_ACTIVE_TRANSACTION              = 1192,
    MOD_MYSQL_MSG_UNKNOWN_SYSTEM_VARIABLE                 = 1193,
    MOD_MYSQL_MSG_CRASHED_ON_USAGE                        = 1194,
    MOD_MYSQL_MSG_CRASHED_ON_REPAIR                       = 1195,
    MOD_MYSQL_MSG_WARNING_NOT_COMPLETE_ROLLBACK           = 1196,
    MOD_MYSQL_MSG_TRANS_CACHE_FULL                        = 1197,
    MOD_MYSQL_MSG_SLAVE_MUST_STOP                         = 1198,
    MOD_MYSQL_MSG_SLAVE_NOT_RUNNING                       = 1199,
    MOD_MYSQL_MSG_BAD_SLAVE                               = 1200,
    MOD_MYSQL_MSG_MASTER_INFO                             = 1201,
    MOD_MYSQL_MSG_SLAVE_THREAD                            = 1202,
    MOD_MYSQL_MSG_TOO_MANY_USER_SERVER_MESSAGES           = 1203,
    MOD_MYSQL_MSG_SET_CONSTANTS_ONLY                      = 1204,
    MOD_MYSQL_MSG_LOCK_WAIT_TIMEOUT                       = 1205,
    MOD_MYSQL_MSG_LOCK_TABLE_FULL                         = 1206,
    MOD_MYSQL_MSG_READ_ONLY_TRANSACTION                   = 1207,
    MOD_MYSQL_MSG_DROP_DB_WITH_READ_LOCK                  = 1208,
    MOD_MYSQL_MSG_CREATE_DB_WITH_READ_LOCK                = 1209,
    MOD_MYSQL_MSG_WRONG_ARGUMENTS                         = 1210,
    MOD_MYSQL_MSG_NO_PERMISSION_TO_CREATE_USER            = 1211,
    MOD_MYSQL_MSG_UNION_TABLES_IN_DIFFERENT_DIR           = 1212,
    MOD_MYSQL_MSG_LOCK_DEADLOCK                           = 1213,
    MOD_MYSQL_MSG_TABLE_CANT_HANDLE_FT                    = 1214,
    MOD_MYSQL_MSG_CANNOT_ADD_FOREIGN                      = 1215,
    MOD_MYSQL_MSG_NO_REFERENCED_ROW                       = 1216,
    MOD_MYSQL_MSG_ROW_IS_REFERENCED                       = 1217,
    MOD_MYSQL_MSG_CONNECT_TO_MASTER                       = 1218,
    MOD_MYSQL_MSG_QUERY_ON_MASTER                         = 1219,
    MOD_MYSQL_MSG_ERROR_WHEN_EXECUTING_COMMAND            = 1220,
    MOD_MYSQL_MSG_WRONG_USAGE                             = 1221,
    MOD_MYSQL_MSG_WRONG_NUMBER_OF_COLUMNS_IN_SELECT       = 1222,
    MOD_MYSQL_MSG_CANT_UPDATE_WITH_READLOCK               = 1223,
    MOD_MYSQL_MSG_MIXING_NOT_ALLOWED                      = 1224,
    MOD_MYSQL_MSG_DUP_ARGUMENT                            = 1225,
    MOD_MYSQL_MSG_USER_LIMIT_REACHED                      = 1226,
    MOD_MYSQL_MSG_SPECIFIC_ACCESS_DENIED_ERROR            = 1227,
    MOD_MYSQL_MSG_LOCAL_VARIABLE                          = 1228,
    MOD_MYSQL_MSG_GLOBAL_VARIABLE                         = 1229,
    MOD_MYSQL_MSG_NO_DEFAULT                              = 1230,
    MOD_MYSQL_MSG_WRONG_VALUE_FOR_VAR                     = 1231,
    MOD_MYSQL_MSG_WRONG_TYPE_FOR_VAR                      = 1232,
    MOD_MYSQL_MSG_VAR_CANT_BE_READ                        = 1233,
    MOD_MYSQL_MSG_CANT_USE_OPTION_HERE                    = 1234,
    MOD_MYSQL_MSG_NOT_SUPPORTED_YET                       = 1235,
    MOD_MYSQL_MSG_MASTER_FATAL_ERROR_READING_BINLOG       = 1236,
    MOD_MYSQL_MSG_SLAVE_IGNORED_TABLE                     = 1237,
    MOD_MYSQL_MSG_INCORRECT_GLOBAL_LOCAL_VAR              = 1238,
    MOD_MYSQL_MSG_WRONG_FK_DEF                            = 1239,
    MOD_MYSQL_MSG_KEY_REF_DO_NOT_MATCH_TABLE_REF          = 1240,
    MOD_MYSQL_MSG_OPERAND_COLUMNS                         = 1241,
    MOD_MYSQL_MSG_SUBQUERY_NO_1_ROW                       = 1242,
    MOD_MYSQL_MSG_UNKNOWN_STMT_HANDLER                    = 1243,
    MOD_MYSQL_MSG_CORRUPT_HELP_DB                         = 1244,
    MOD_MYSQL_MSG_CYCLIC_REFERENCE                        = 1245,
    MOD_MYSQL_MSG_AUTO_CONVERT                            = 1246,
    MOD_MYSQL_MSG_ILLEGAL_REFERENCE                       = 1247,
    MOD_MYSQL_MSG_DERIVED_MUST_HAVE_ALIAS                 = 1248,
    MOD_MYSQL_MSG_SELECT_REDUCED                          = 1249,
    MOD_MYSQL_MSG_TABLENAME_NOT_ALLOWED_HERE              = 1250,
    MOD_MYSQL_MSG_NOT_SUPPORTED_AUTH_MODE                 = 1251,
    MOD_MYSQL_MSG_SPATIAL_CANT_HAVE_NULL                  = 1252,
    MOD_MYSQL_MSG_COLLATION_CHARSET_MISMATCH              = 1253,
    MOD_MYSQL_MSG_SLAVE_WAS_RUNNING                       = 1254,
    MOD_MYSQL_MSG_SLAVE_WAS_NOT_RUNNING                   = 1255,
    MOD_MYSQL_MSG_TOO_BIG_FOR_UNCOMPRESS                  = 1256,
    MOD_MYSQL_MSG_ZLIB_Z_MEM_ERROR                        = 1257,
    MOD_MYSQL_MSG_ZLIB_Z_BUF_ERROR                        = 1258,
    MOD_MYSQL_MSG_ZLIB_Z_DATA_ERROR                       = 1259,
    MOD_MYSQL_MSG_CUT_VALUE_GROUP_CONCAT                  = 1260,
    MOD_MYSQL_MSG_WARN_TOO_FEW_RECORDS                    = 1261,
    MOD_MYSQL_MSG_WARN_TOO_MANY_RECORDS                   = 1262,
    MOD_MYSQL_MSG_WARN_NULL_TO_NOTNULL                    = 1263,
    MOD_MYSQL_MSG_WARN_DATA_OUT_OF_RANGE                  = 1264,
    MOD_MYSQL_MSG_WARN_DATA_TRUNCATED                     = 1265,
    MOD_MYSQL_MSG_WARN_USING_OTHER_HANDLER                = 1266,
    MOD_MYSQL_MSG_CANT_AGGREGATE_2COLLATIONS              = 1267,
    MOD_MYSQL_MSG_DROP_USER                               = 1268,
    MOD_MYSQL_MSG_REVOKE_GRANTS                           = 1269,
    MOD_MYSQL_MSG_CANT_AGGREGATE_3COLLATIONS              = 1270,
    MOD_MYSQL_MSG_CANT_AGGREGATE_NCOLLATIONS              = 1271,
    MOD_MYSQL_MSG_VARIABLE_IS_NOT_STRUCT                  = 1272,
    MOD_MYSQL_MSG_UNKNOWN_COLLATION                       = 1273,
    MOD_MYSQL_MSG_SLAVE_IGNORED_SSL_PARAMS                = 1274,
    MOD_MYSQL_MSG_SERVER_IS_IN_SECURE_AUTH_MODE           = 1275,
    MOD_MYSQL_MSG_WARN_FIELD_RESOLVED                     = 1276,
    MOD_MYSQL_MSG_BAD_SLAVE_UNTIL_COND                    = 1277,
    MOD_MYSQL_MSG_MISSING_SKIP_SLAVE                      = 1278,
    MOD_MYSQL_MSG_UNTIL_COND_IGNORED                      = 1279,
    MOD_MYSQL_MSG_WRONG_NAME_FOR_INDEX                    = 1280,
    MOD_MYSQL_MSG_WRONG_NAME_FOR_CATALOG                  = 1281,
    MOD_MYSQL_MSG_WARN_QC_RESIZE                          = 1282,
    MOD_MYSQL_MSG_BAD_FT_COLUMN                           = 1283,
    MOD_MYSQL_MSG_UNKNOWN_KEY_CACHE                       = 1284,
    MOD_MYSQL_MSG_WARN_HOSTNAME_WONT_WORK                 = 1285,
    MOD_MYSQL_MSG_UNKNOWN_STORAGE_ENGINE                  = 1286,
    MOD_MYSQL_MSG_WARN_DEPRECATED_SYNTAX                  = 1287,
    MOD_MYSQL_MSG_NON_UPDATABLE_TABLE                     = 1288,
    MOD_MYSQL_MSG_FEATURE_DISABLED                        = 1289,
    MOD_MYSQL_MSG_OPTION_PREVENTS_STATEMENT               = 1290,
    MOD_MYSQL_MSG_DUPLICATED_VALUE_IN_TYPE                = 1291,
    MOD_MYSQL_MSG_TRUNCATED_WRONG_VALUE                   = 1292,
    MOD_MYSQL_MSG_TOO_MUCH_AUTO_TIMESTAMP_COLS            = 1293,
    MOD_MYSQL_MSG_INVALID_ON_UPDATE                       = 1294,
    MOD_MYSQL_MSG_UNSUPPORTED_PS                          = 1295,
    MOD_MYSQL_MSG_GET_ERRMSG                              = 1296,
    MOD_MYSQL_MSG_GET_TEMPORARY_ERRMSG                    = 1297,
    MOD_MYSQL_MSG_UNKNOWN_TIME_ZONE                       = 1298,
    MOD_MYSQL_MSG_WARN_INVALID_TIMESTAMP                  = 1299,
    MOD_MYSQL_MSG_INVALID_CHARACTER_STRING                = 1300,
    MOD_MYSQL_MSG_WARN_ALLOWED_PACKET_OVERFLOWED          = 1301,
    MOD_MYSQL_MSG_CONFLICTING_DECLARATIONS                = 1302
};

#ifndef MOD_MYSQL_SERVER_MESSAGE_T_DEFINED
typedef struct mod_mysql_server_message_t mod_mysql_server_message_t;
#define MOD_MYSQL_SERVER_MESSAGE_T_DEFINED
#endif /* MOD_MYSQL_SERVER_MESSAGE_T_DEFINED */

struct mod_mysql_server_message_t
{
    mod_mysql_server_message_code_t code;
    const char* msg;
};

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_instantiable.h"

class mod_mysql_server_message
        : public mod_mysql_base,
          public mod_mysql_server_message_t,
          public mod_mysql_instantiable
{
public:
    typedef mod_mysql_server_message_code_t code_t;

public:
    mod_mysql_server_message(const code_t code = MOD_MYSQL_MSG_NONE,
            const char* msg = 0);
    const code_t& get_code() const;
    const char* get_message() const;
};

// {{{ mod_mysql_server_message::mod_mysql_server_message()
inline mod_mysql_server_message::mod_mysql_server_message(const code_t _code,
        const char* _msg)
{
    code = _code;
    msg = _msg;
}
// }}}

// {{{ mod_mysql_server_message::get_code()
inline const mod_mysql_server_message::code_t&
mod_mysql_server_message::get_code() const
{
    return code;
}
// }}}

// {{{ mod_mysql_server_message::get_message()
inline const char* mod_mysql_server_message::get_message() const
{
    return msg;
}
// }}}

#endif

#endif /* _MOD_MYSQL_SERVER_MESSAGE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
