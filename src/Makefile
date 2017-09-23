STATIC_LIB = libamq.a

DESTDIR=../../
PREFIX=../

SRC =  \
  AMessageQueue.cpp \
  socket/socket_utils.cpp\
  socket/tcp_io.cpp        \
  socket/tcp_protocol.cpp \
  utils/string_utils.cpp

OBJ = $(SRC:.cpp=.o)

CXX = clang++
CXXFLAGS = -Wall -std=c++11 $(DEBUG)
CXXFLAGS += -I../include/

LIBS    = -lev

$(STATIC_LIB): $(OBJ)
	@echo "[Link (Static)]"
	@echo ${OBJ}
	@ar rcs $@ $^

.cpp.o:
	@echo [Compile] $<
	@$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags *.bak Makefile.bak libgeniePi.*

.PHONY : install

	mkdir -p ../lib && cp $< ../lib/
install: $(STATIC_LIB)
	mkdir -p ../lib && cp $(STATIC_LIB) ../lib/
	@install -m 0755 $< $(DESTDIR)$(PREFIX)/lib
all : install

