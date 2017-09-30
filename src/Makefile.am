AUTOMAKE_OPTIONS=foreign

lib_LIBRARIES = libamq.a

libamq_a_SOURCES = \
    AMessageQueue.cpp \
    socket/socket_utils.cpp \
    socket/tcp_io.cpp \
    socket/tcp_protocol.cpp \
    utils/string_utils.cpp
libamq_a_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include

if DEBUG
AM_CFLAGS = -g3 -O0
AM_CXXFLAGS = -g3 -O0
else
AM_CFLAGS = -O2
AM_CXXFLAGS = -O2
endif