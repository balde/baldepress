lib_LTLIBRARIES = libbaldepress.la

libbaldepress_la_SOURCES = \
	git.c \
	markdown.c

libbaldepress_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBGIT2_CFLAGS) \
	-I$(top_srcdir)/include

libbaldepress_la_LDFLAGS = \
	$(AM_LDADD) \
	$(GLIB_LIBS) \
	$(LIBGIT2_LIBS) \
	-version-info $(LT_VERSION_INFO)
