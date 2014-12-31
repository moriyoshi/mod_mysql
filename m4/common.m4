AC_DEFUN([MOD_MYSQL_CHECK_LIB_AVAILABILITY], [
  name="$1"
  ac_filename_desc="`eval echo $libname_spec`(.$libext|$shrext_cmds)"
  AC_MSG_CHECKING([$ac_filename_desc availability])
  if eval test ! -e "$_dir/$libname_spec$shrext_cmds" -a ! -e "$_dir/$libname_spec.$libext"; then
    AC_MSG_RESULT([no])
    $3
  else
    AC_MSG_RESULT([yes])
    $2
  fi
])

dnl vim600: sts=2 sw=2 ts=2 et
