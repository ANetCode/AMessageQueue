#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([amq], [0.1.0], [bug-report@2polygon.com])
AM_INIT_AUTOMAKE([foreign -Werror subdir-objects])

AC_PROG_CXX
AC_PROG_RANLIB

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

AC_CONFIG_SRCDIR([])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile \
    src/Makefile \
    Tests/Makefile \
    Tests/dummy/Makefile \
    Tests/server/Makefile \
    Tests/client/Makefile
])

AC_OUTPUT
