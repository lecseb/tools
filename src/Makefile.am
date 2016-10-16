# This file is part of libtools
#
# Foobar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with libtools.  If not, see <http:www.gnu.org/licenses/>.

lib_LTLIBRARIES= libtools.la

libtools_la_CFLAGS= -fPIC -I$(top_builddir)/include
libtools_la_LDFLAGS= -shared -fPIC

libtools_la_SOURCES= \
	m_alloc.c \
	s_list.c \
	s_queue.c \
	s_stack.c \
	tree/s_rb_tree.c \
	tree/s_rb_tree-add.c \
	tree/s_rb_tree-private.c \
	tree/s_rb_tree-remove.c

CLEANFILES= *~ \#*\#
