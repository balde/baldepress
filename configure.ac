AC_PREREQ(2.68)

AC_INIT([baldepress], m4_esyscmd([build-aux/git-version-gen .tarball-version]),
    [https://github.com/rafaelmartins/baldepress], [baldepress],
    [https://github.com/rafaelmartins/baldepress])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz subdir-objects])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, baldepress requires a C99 compiler.])
])

AC_SEARCH_LIBS([mkd_in], [markdown], ,
  AC_MSG_ERROR([mkd_in not found. please install the discount markdown parser library.])
)

AC_ARG_WITH([valgrind], AS_HELP_STRING([--without-valgrind],
            [ignore presence of valgrind]))
AS_IF([test "x$with_valgrind" != "xno"], [
  AC_PATH_PROG([valgrind], [valgrind])
  AS_IF([test "x$ac_cv_path_valgrind" = "x"], [
    have_valgrind=no
  ], [
    have_valgrind=yes
  ])
])
AS_IF([test "x$have_valgrind" = "xyes"], , [
  AS_IF([test "x$with_valgrind" = "xyes"], [
    AC_MSG_ERROR([valgrind requested but not found])
  ])
])
AM_CONDITIONAL([USE_VALGRIND], [test "x$have_valgrind" = "xyes"])
VALGRIND="$ac_cv_path_valgrind"
AC_SUBST(VALGRIND)

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.34])
PKG_CHECK_MODULES([BALDE], [balde])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/include/Makefile
    src/libbaldepress/Makefile
    tests/Makefile
])
AC_OUTPUT

AS_ECHO("
        ==== ${PACKAGE_STRING} ====

        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}

        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}
")
