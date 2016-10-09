noinst_LTLIBRARIES = libcassandra_api.la
libcassandra_api_la_SOURCES = cassandra_api.c
libcassandra_api_la_CFLAGS = -I$(top_src)/include
libcassandra_api_la_LDFLAGS = -lcassandra
#libcassandra_api_la_LDADD =

bin_PROGRAMS = test_cassandra_api
test_cassandra_api_SOURCES = test_cassandra_api.c
test_cassandra_api_CFLAGS = -I$(top_srcdir)/include\
							-I$(top_srcdir)/libs/stacks/cassandra
test_cassandra_api_LDFLAGS = -lcassandra
test_cassandra_api_LDADD = $(top_builddir)/libs/stacks/cassandra/libcassandra_api.la
