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

lib_LTLIBRARIES= libtools.la

libtools_la_CFLAGS= -fPIC -I$(top_builddir)/include
libtools_la_LDFLAGS= -shared -fPIC

libtools_la_SOURCES= \
	m_alloc.c \
	list/s_d_list.c \
	list/s_stack.c \
	queue/s_queue.c \
	queue/s_ordered_queue.c \
	tree/s_rb_tree.c \
	tree/s_rb_tree-add.c \
	tree/s_rb_tree-private.c \
	tree/s_rb_tree-remove.c

libtools_la_HEADERS= \
	$(top_builddir)/include/m_alloc.h \
	$(top_builddir)/include/m_export.h \
	$(top_builddir)/include/t_funcs.h \
	$(top_builddir)/include/m_print.h \
	$(top_builddir)/include/m_utils.h \
	$(top_builddir)/include/list/s_d_list.h \
	$(top_builddir)/include/list/s_stack.h \
	$(top_builddir)/include/queue/s_queue.h \
	$(top_builddir)/include/queue/s_ordered_queue.h \
	$(top_builddir)/include/tree/s_rb_tree.h

libtools_ladir= \
	$(top_builddir)/include/

CLEANFILES= *~ \#*\#

codecheck:
	@for file in $(libtools_la_SOURCES) $(libtools_la_HEADERS); do \
		echo $$file; \
		$(top_builddir)/script/checkpatch.pl --no-tree --no-summary --terse \
			--show-types -f --ignore SPLIT_STRING,BRACES $$file; \
	done


