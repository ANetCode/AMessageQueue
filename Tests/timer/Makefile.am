bin_PROGRAMS = timertest

timertest_SOURCES = main.cpp
timertest_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include
timertest_LDFLAGS  = -L$(top_srcdir)/src/ -lamq -lev
timertest_LDADD = $(top_srcdir)/src/libamq.a

AM_CFLAGS = -g3 -O0
AM_CXXFLAGS = -g3 -O0
