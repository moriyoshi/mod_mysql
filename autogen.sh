aclocal -I m4
libtoolize -c --force
autoheader
automake -c -a --foreign
autoconf
