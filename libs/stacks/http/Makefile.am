noinst_LTLIBRARIES = libstack_http.la

libstack_http_la_CFLAGS = -I$(top_srcdir)/include             \
                          -I$(top_srcdir)/libs/stacks/rauthen \
                          -I$(top_srcdir)/libs/sdf            \
                          -I$(top_srcdir)/libs/codecs/urlencode

libstack_http_la_LIBADD = $(top_builddir)/libs/codecs/urlencode/liburlencode.la

libstack_http_la_SOURCES = http.h http.c
