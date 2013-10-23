#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cpp-tango], [1.0])
AC_CONFIG_SRCDIR([card.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX([c++])
AX_CXX_COMPILE_STDCXX_11([noext])
AX_BOOST_BASE
AX_BOOST_DATE_TIME
AX_BOOST_SERIALIZATION
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline], [readline])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([tzset])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
