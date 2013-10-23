#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([tango], [1.0])
AC_CONFIG_SRCDIR([card.cpp])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext])

# Checks for libraries.
AC_CHECK_LIB([readline], [readline])
AX_BOOST_BASE
AX_BOOST_SERIALIZATION
AX_BOOST_DATE_TIME

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
