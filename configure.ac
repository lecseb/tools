# This file is part of libtools
#
# libtools is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# libtools is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with libtools.  If not, see <http:www.gnu.org/licenses/>.

AC_PREREQ([2.68])
AC_INIT([libtools], [0.1], [lecseb@toto.com])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_LIBTOOL
AC_ENABLE_SHARED
AC_DISABLE_STATIC

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

AC_ARG_ENABLE([debug],AS_HELP_STRING([--enable-debug],[Debug flags]),
	[enable_debug=$enableval],[enable_debug="no"])
AC_MSG_CHECKING(debug)
AC_MSG_RESULT($enable_debug)

if test "x$enable_debug" = "xyes"
then
	CFLAGS+=" -g -ggdb -DDEBUG "
fi

CFLAGS+=" -W -Wall -Werror -fvisibility=hidden "

AC_CONFIG_FILES([
	Makefile \
	src/Makefile])

AC_OUTPUT
