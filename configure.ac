#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([amq], [0.1.0], [bug-report@2polygon.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])

AC_PROG_CXX
AC_PROG_RANLIB

CXXFLAGS="$CXXFLAGS -std=c++0x"

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
