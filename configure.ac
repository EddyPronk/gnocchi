# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gnocci, 1.1, janneke@gnu.org)
AM_INIT_AUTOMAKE
#AC_CONFIG_SRCDIR([bar/boos.cc])
AC_CONFIG_FILES([Makefile])

CFLAGS=${CFLAGS-"-Wall -Wextra -fprofile-arcs -ftest-coverage -DIN_GCC"}
CXXFLAGS="$CXXFLAGS $CFLAGS"

AC_CANONICAL_HOST
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AM_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_OUTPUT
