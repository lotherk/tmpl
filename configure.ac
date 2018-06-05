#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(tmpl, 0.1, bugs+tmpl@hiddenbox.org, [], [https://github.com/lotherk/tmpl])
AM_INIT_AUTOMAKE([subdir-objects no-dependencies])

AC_SUBST([COPYRIGHT_NOTICE], ["Copyright (C) 2018 Konrad Lother <k@hiddenbox.org>"])
AC_SUBST([DEFAULT_PROGRAM], [/bin/sh])

AC_DEFINE([MKSTEMP_TEMPLATE], ["/tmp/.tmpl-XXXXXXXXXX"], ["Default mkstemp(3) template"])

AC_LANG([C])

AC_CONFIG_SRCDIR([src/tmpl.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([tmpl.ggo man/tmpl.1])

# Checks for programs.
AC_PROG_CC([clang cc gcc])
AC_PROG_INSTALL
AC_PROG_MAKE_SET
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([\
                  sys/filio.h \
                  sys/select.h \
                  ])

AC_CHECK_HEADERS([\
                  err.h \
                  limits.h \
                  stdio.h \
                  stdlib.h \
                  string.h \
                  sys/ioctl.h \
                  sys/stat.h \
                  sys/wait.h \
                  time.h \
                  unistd.h \
                  ], [],
[AC_MSG_ERROR(["*** error: missing header file ***"])])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([\
                pledge \
                ])

AC_CHECK_FUNCS([\
                atexit \
                dup2 \
                setenv \
                strchr \
                strdup \
                err \
                warn \
                nanosleep \
                fchmod \
                ], [],
[AC_MSG_ERROR(["*** error: missing function ***"])])

AC_CHECK_MEMBERS([struct timespec.tv_sec, struct timespec.tv_nsec])

AC_OUTPUT(Makefile src/Makefile man/Makefile)

