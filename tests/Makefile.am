check_PROGRAMS = 

TESTS = \
	$(check_PROGRAMS)



AM_CFLAGS = \
	$(all_includes) \
	$(BALDE_CFLAGS) \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)/src/include

AM_LDFLAGS = \
	$(BALDE_CFLAGS) \
	$(GLIB_LIBS) \
	$(top_builddir)/src/libbaldepress/libbaldepress.la

if USE_VALGRIND
valgrind:
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"
endif
