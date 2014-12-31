AC_DEFUN([MOD_MYSQL_CHECK_APXS], [
  APXS=
  AC_MSG_CHECKING([apxs location])
  for _path in m4_translit($1, [
], [ ]); do
    if test -x "$_path"; then
      APXS="$_path"
      break
    fi
  done

  if test -z "$APXS"; then
    AC_MSG_RESULT([not found])
    $3
  else
    AC_MSG_RESULT([$APXS])
    $2
  fi
])

dnl vim600: sts=2 sw=2 ts=2 et
