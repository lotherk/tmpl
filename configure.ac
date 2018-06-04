#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(tmpl, 0.1, bugs+tmpl@hiddenbox.org)
AM_INIT_AUTOMAKE(tmpl, 0.1)
AC_CONFIG_SRCDIR([src/tmpl.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([pledge atexit dup2 setenv strchr strdup])

AC_OUTPUT(Makefile src/Makefile man/Makefile)
