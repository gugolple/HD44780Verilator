m4_define([nu_version],
          m4_esyscmd([./package-version . version-stamp]))
AC_INIT([vtesthd44780], [m4_defn([nu_version])], [juanignacio.croce@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# This is run at the where the config is executed, being done within the
# actual compilation environment, that is the reason of an extra folder up
SRCSVERSION=`../package-version ../../fpga_gowin_hd44780/ version-stamp`
AC_DEFINE_UNQUOTED([SRCSVERSION],
                   ["$SRCSVERSION"],
                   [Define SRCS revision number])

# Debug flag
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
# End debug flag

# Get Verilog target name fron environment
AC_SUBST(TGT, ${TGT})

AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
AC_PROG_RANLIB
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
])
AC_OUTPUT
