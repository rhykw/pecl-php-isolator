dnl $Id$
PHP_ARG_ENABLE(isolator, whether to enable isolator support, 
[  --enable-isolator          Enable isolator support])

if test "$PHP_ISOLATOR" != "no"; then
  AC_DEFINE(HAVE_ISOLATOR, 1, [ ])
  PHP_NEW_EXTENSION(isolator, isolator.c, $ext_shared)
fi
