sinclude(m4/common.m4)

AC_DEFUN([MOD_MYSQL_CHECK_ICU], [
  icu_includes=
  icu_lib_path= 
  icu_prefix=

  for _pfx in m4_translit($1, [
], [ ]); do
    _include_dir="$_pfx/include"
    for _dir in "$_include_dir" "$_include_dir/unicode" "$_include_dir/icu"; do
      AC_MSG_CHECKING([uversion.h presence in $_dir])
      if test -e "$_dir/uversion.h"; then
        AC_MSG_RESULT([yes])
        ac_save_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS -I$_include_dir -I$_dir"
        AC_CHECK_HEADERS([uversion.h], [
          if test "$_include_dir" != "$_dir"; then
            icu_includes="$icu_includes -I$_include_dir"
          fi
          icu_includes="$icu_includes -I$_dir"
          icu_prefix="$_pfx"
        ])
        CPPFLAGS="$ac_save_CPPFLAGS"

        if test ! -z "$icu_includes"; then
          break
        fi
      else
        AC_MSG_RESULT([no])
      fi
    done

    if test ! -z "$icu_includes"; then
      break
    fi
  done

  if test -z "$icu_includes"; then
    AC_MSG_ERROR([Cannot find ICU headers.])
  fi
 
  for _dir in "$icu_prefix/lib" "$icu_prefix/lib/unicode" "$icu_prefix/lib/icu"; do
    MOD_MYSQL_CHECK_LIB_AVAILABILITY([icuuc], [], [continue])
    MOD_MYSQL_CHECK_LIB_AVAILABILITY([icui18n], [], [continue])
    MOD_MYSQL_CHECK_LIB_AVAILABILITY([icudata], [], [continue])

    AC_MSG_CHECKING([u_getVersion() usability])
    ac_save_LIBS="$LIBS"
    ac_save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $icu_includes"
    LIBS="$LIBS -L$_dir -licuuc -licudata -licui18n"

    AC_TRY_LINK([
#include <uversion.h>
    ], [
      UVersionInfo ver;
      u_getVersion(ver);
    ], [
      AC_MSG_RESULT([yes])
      icu_lib_path="$_dir"
    ], [
      AC_MSG_RESULT([no])
    ])

    LIBS="$ac_save_LIBS"
    CPPFLAGS="$ac_save_CPPFLAGS"

    if test ! -z "$icu_lib_path"; then
      break
    fi
  done

  if test -z "$icu_lib_path"; then
    AC_MSG_ERROR([Cannot find ICU libraries.])
  fi

  INCLUDES="$INCLUDES $icu_includes"
  LIBS="$LIBS -L$icu_lib_path -licuuc -licudata -licui18n"
])

dnl vim600: sts=2 sw=2 ts=2 et
