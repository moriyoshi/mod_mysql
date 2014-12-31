lib_LTLIBRARIES=mod_mysql.la
mod_mysql_la_LDFLAGS=-module -avoid-version
mod_mysql_la_SOURCES=\
	mod_mysql_config.h \
	mod_mysql_config.cpp \
	mod_mysql_connection.h \
	mod_mysql_connection.cpp \
	mod_mysql.h \
	mod_mysql.cpp \
	mod_mysql_opaque_collation.h \
	mod_mysql_opaque_collation.cpp \
	mod_mysql_recoverable_exception.h \
	mod_mysql_server_logger.h \
	mod_mysql_server_logger.cpp \
	mod_mysql_conn_logger.h \
	mod_mysql_conn_logger.cpp \
	mod_mysql_std_logger.h \
	mod_mysql_std_logger.cpp \
	mod_mysql_config_exception.h \
	mod_mysql_config_handler.h \
	mod_mysql_command_handler.h \
	mod_mysql_command_handler.cpp \
	mod_mysql_service.h \
	mod_mysql_service_factory.h
mod_mysql_la_LIBADD=@top_srcdir@/lib/libmod_mysql_common.la @LIBS@
NOINST=mod_mysql.la
INCLUDES=-I@top_srcdir@/lib @INCLUDES@
libdir=@MODULEDIR@
