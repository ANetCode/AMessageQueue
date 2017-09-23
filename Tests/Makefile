program = test.app

DESTDIR=../../
PREFIX=../

SRC =  \
  client.cpp \
  server.cpp \
  main.cpp

OBJ = $(SRC:.cpp=.o)

CXX = clang++
CXXFLAGS = -Wall -std=c++11 $(DEBUG)
CXXFLAGS += -I../include/ -lamq -L../lib/ -lev

LIBS    = -lev

$(program): $(OBJ)
	@echo "[Link (program)]"
	$(CXX) ${OBJ} -o $@ $(CXXFLAGS)

.cpp.o:
	@echo [Compile] $<
	@$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags *.bak Makefile.bak libgeniePi.*

.PHONY : install

	mkdir -p ../lib && cp $< ../lib/

all : $(program)

