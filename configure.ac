AC_PREREQ([2.69])
AC_INIT([interflop], [0.1.0], [])
AM_SILENT_RULES([yes])
AC_CONFIG_AUX_DIR(autoconf)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AM_INIT_AUTOMAKE([subdir-objects silent-rules foreign])
AC_PROG_LIBTOOL

LT_INIT
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_LIB([c], [exit], , AC_MSG_ERROR([Could not find c library]))

# Check for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h stdbool.h], , AC_MSG_ERROR([Missing required headers]))

# Generate config files

AC_CONFIG_FILES([Makefile
		 backends/Makefile
		 backends/ieee/Makefile
                ])
AC_OUTPUT
