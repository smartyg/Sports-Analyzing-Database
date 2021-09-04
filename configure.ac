#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sad], [0.0.1], [goedhart.martijn@gmail.com])
AC_CONFIG_SRCDIR([src/sadfs/src/fuse_sadfs.cpp])
AC_CONFIG_HEADERS([config.h])
AH_BOTTOM([#include <features.h>])

AC_CANONICAL_BUILD
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])
AM_GNU_GETTEXT_VERSION([0.19.8])
AM_GNU_GETTEXT([external])

# AX_DEFINE_SUBST(NAME, VALUE, DESCRIPTION)
# -----------------------------------------
AC_DEFUN([AX_DEFINE_SUBST], [
	AC_DEFINE([$1], ["$2"], [$3])
	AC_SUBST([$1], ['$2'])
])

# AX_CHECK_LIB(NAME, FUNC, NAME2)
# -----------------------------------------
AC_DEFUN([AX_CHECK_LIB], [
	AC_CHECK_LIB([$1], [$2], [AC_SUBST([$3_LIBS], ["-l$1"])
	AC_DEFINE([HAVE_LIB$3], [1], [Define if you have lib$1])])
])

# some other package constants
AX_DEFINE_SUBST([PROGRAM_NAME], [Sports Analyzing Database], [program name])
AX_DEFINE_SUBST([PROGRAM_SUMMARY], [Database system to store and analyze recorded sport trainings], [program summary])
AX_DEFINE_SUBST([PROGRAM_DESCRIPTION], [...], [program description])
AX_DEFINE_SUBST([LAST_CHANGE], [01-09-2021], [Date of last change])
AX_DEFINE_SUBST([AUTHOR], [Martijn Goedhart], [Author])
AX_DEFINE_SUBST([COPYRIGHT], [Copyright (C) 2021 Martijn Goedhart], [Copyright])
AX_DEFINE_SUBST([LICENSE], [GNU General Public License], [License name])
AX_DEFINE_SUBST([LICENSE_SHORT], [GPLv2+], [License shorthand])
AX_DEFINE_SUBST([LICENSE_SHORT_RPM], [GPL-2.0+], [SPDX License code])
AX_DEFINE_SUBST([LICENSE_LONG], [GNU GPL version 2 or later <http://www.gnu.org/licenses/gpl-2.0.html>], [Long license description])
AX_DEFINE_SUBST([PACKAGE_HOMEPAGE], [http://www.martijn-goedhart.nl/sad/], [package home page])
AX_DEFINE_SUBST([LIBRARY_CURRENT_VERSION], [0], [version of this library])
AX_DEFINE_SUBST([LIBRARY_REVISION_VERSION], [0], [version of this library])
AX_DEFINE_SUBST([LIBRARY_AGE_VERSION], [0], [version of this library])
AX_DEFINE_SUBST([LIBRARY_API_VERSION], [0], [version of this library])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# this allows us specify individual liking flags for each target
AM_PROG_CC_C_O

# Initialize Libtool
LT_INIT

# Check if Libtool is present
# Libtool is used for building share libraries
AC_PROG_LIBTOOL

# Check which option are enabled and disabled
# use debuging
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug], [Turn on debugging @<:@default=disabled@:>@])],
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	esac],
	[debug=false])

#compile manpage?
AC_ARG_ENABLE([man],
	[AS_HELP_STRING([--disable-man], [generate documentation @<:@default=enabled@:>@])],
	[case "${enableval}" in
		yes) man=true ;;
		no)  man=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --disable-man]) ;;
	esac],
	[man=true])

# Define large file symbol
AC_SYS_LARGEFILE

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h sys/stat.h locale.h time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset setlocale strcpy])

# Checks for libraries.

# Checks for programs.
AC_CHECK_PROGS([GIT], [git])

# Checks for packages.
PKG_CHECK_MODULES([FUSE], [fuse >= 2.9.7])
AX_CHECK_LIB([dl], [dlopen], [DL])

# AC_DEFINE([_GNU_SOURCE], [], [use gnu extensions])
AC_DEFINE([_(s)], [gettext(s)], [use gnu extensions])

# Compute the major version of the library which is the last part of the library SONAME.
AC_SUBST([LIBRARY_OLD_INTERFACE], [$((LIBRARY_CURRENT_VERSION-LIBRARY_AGE_VERSION))])

if test "x$debug" == xtrue; then
	AC_DEFINE([DEBUG], [], [debug flag])
else
	AM_SILENT_RULES([yes])
fi

AC_SUBST(have_dot, "NO")
if test "x$docs" == xtrue; then
	AC_CHECK_PROGS([DOXYGEN], [doxygen])
	if test -z "$DOXYGEN"
	then
		AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
		docs=false
	else
		AC_CHECK_PROGS([DOT], [dot])
		if test -z "$DOT"
		then
			AC_MSG_WARN([Dot not found - continuing without Dot support])
		else
			AC_SUBST(have_dot, "YES")
		fi
	fi
fi

if test "x$man" == xtrue; then
	AC_CHECK_PROGS([HELP2MAN], [help2man])

	if test -z "$HELP2MAN"
	then
		AC_MSG_WARN([help2man not found - continuing without man-page support])
		man=false
	fi
fi

if test -f sad.spec.in
then
	AC_CONFIG_FILES([sad.spec])
	git_version=$VERSION
	if test -n "$GIT"
	then
		git_version=$($GIT describe --tags --match='v@<:@0-9@:>@*' --dirty=-modified | $SED -E -e 's/^v(@<:@0-9@:>@@<:@.0-9@:>@*)(-(@<:@0-9@:>@+)-(@<:@0-9a-g@:>@+))?(-(modified))?/\1+git\3.\4.\6/' -e 's/@<:@.@:>@{2,}/./' -e 's/(\+git)?@<:@.@:>@$//')
		if test $? -ne 0
		then
			git_version=$VERSION
		fi
	fi
	AC_SUBST([GIT_VERSION], [$git_version])
fi

if test -f README.in; then
	AC_CONFIG_FILES([README])
fi

AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AM_CONDITIONAL([DOCS], [test x$docs = xtrue])
AM_CONDITIONAL([MAN], [test x$man = xtrue])
AM_CONDITIONAL([HAVE_HELP2MAN], [test -n "$HELP2MAN"])
AM_CONDITIONAL([HAVE_GIT], [test -n "$GIT"])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_CONDITIONAL([HAVE_DOT], [test -n "$DOT"])

AC_CONFIG_FILES([Makefile src/sadfs/po/Makefile.in src/libsad/po/Makefile.in docs/Doxyfile docs/Makefile libsad.pc])
AC_OUTPUT
