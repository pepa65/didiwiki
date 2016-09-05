AC_PREREQ(2.59)
AC_INIT(didiwiki, 0.5, [mallum@handhelds.org])
AC_CONFIG_SRCDIR([src/util.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([asprintf inet_ntoa memset mkdir putenv select socket strdup strncasecmp])

if test "x$GCC" = "xyes"; then
        GCC_FLAGS="-g -Wall -fno-strict-aliasing"
fi

AC_SUBST(GCC_FLAGS)

AC_OUTPUT([
Makefile
src/Makefile
])

echo
echo "  NOTE:"
echo "  If you are upgrading from a earlier version of DidiWiki, delete"
echo "  \$HOME/.didiwiki/WikiHelp to get updated with the latest version."
echo
