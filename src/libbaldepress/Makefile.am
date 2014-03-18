lib_LTLIBRARIES = libbaldepress.la

libbaldepress_la_SOURCES = \
	markdown.c

libbaldepress_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)/src/include

libbaldepress_la_LDFLAGS = \
	$(AM_LDADD) \
	$(GLIB_LIBS) \
	-version-info $(LT_VERSION_INFO)
