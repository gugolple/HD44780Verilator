m4_define([nu_version],
          m4_esyscmd([./package-version . version-stamp]))
AC_INIT([vtesthd44780], [m4_defn([nu_version])], [juanignacio.croce@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
])
AC_OUTPUT
