all: world

CXX?=g++
CXXFLAGS?=--std=c++17 -Wall -fPIC

OBJS:= \
	objs/main.o

LOGGER_DIR:=.
include common/Makefile.inc
include Makefile.inc

world: example

$(shell mkdir -p objs)

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

example: $(COMMON_OBJS) $(LOGGER_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@;

clean:
	@rm -rf objs
	@rm -f example
