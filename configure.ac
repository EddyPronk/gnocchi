# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gnocchi, 0.22, epronk@muftor.com)
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])

CFLAGS=${CFLAGS-"-Wall -Wextra -DIN_GCC"}
CXXFLAGS="$CXXFLAGS $CFLAGS"

AC_CANONICAL_HOST
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AM_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_CONFIG_HEADER([auto-host.h:auto-host.h.in])

AC_LANG_PUSH(C++)
AC_CHECK_HEADERS([boost/graph/graph_utility.hpp], [],
  [AC_MSG_ERROR([please install libboost-graph-dev])])
AC_CHECK_HEADERS([boost/filesystem/path.hpp], [],
  [AC_MSG_ERROR([please install libboost-filesystem-dev])])
AC_LANG_POP(C++)

AC_OUTPUT
