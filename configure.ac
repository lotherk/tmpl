#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(tmpl, 0.1, bugs+tmpl@hiddenbox.org)
AM_INIT_AUTOMAKE([subdir-objects no-dependencies])

AC_LANG([C])

AC_CONFIG_SRCDIR([src/tmpl.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([tmpl.ggo man/tmpl.1])

# Checks for programs.
AC_PROG_CC([clang cc gcc])
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET
# Checks for libraries.

AC_CANONICAL_HOST


# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/ioctl.h unistd.h sys/filio.h])

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

