noinst_LTLIBRARIES = libsigrok_hw_rigol_ds5k.la

libsigrok_hw_rigol_ds5k_la_SOURCES = \
	api.c \
	protocol.c \
	protocol.h

libsigrok_hw_rigol_ds5k_la_CFLAGS = \
	-I$(top_srcdir)/include -I$(top_builddir)/include \
	-I$(top_srcdir)/src \
	$(LIBSIGROK_CFLAGS) \
	$(AM_CFLAGS)

libsigrok_hw_rigol_ds5k_la_LDFLAGS = $(LIBSIGROK_LIBS)

noinst_HEADERS = protocol.h
