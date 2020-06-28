WXLIBS = core,base

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(WX_CXXFLAGS)

OPTFLAGS = \
  -O2 \
  -march=native

DBGFLAGS = \
  -g

LDFLAGS = \
  $(WX_LDFLAGS)

WX_CXXFLAGS = $(shell wx-config --cxxflags | sed "s/-I/-isystem/g")
WX_LDFLAGS = $(shell wx-config --libs $(WXLIBS))

SOURCES = \
  $(wildcard ./*.cpp) \
  $(wildcard */*.cpp)

HEADERS = \
  $(wildcard ./*.h) \
  $(wildcard */*.h)

OBJECTS = \
  $(SOURCES:.cpp=.o)

ARTIFACTS = \
  main \
  _deps.mk \
  $(OBJECTS) \


.PHONY: all clean deps fmt
.SUFFIXES: .cpp

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

deps:
	for src in $(SOURCES); do \
	    $(CXX) $(CXXFLAGS) -MM -MF- -MT $${src%.cpp}.o $${src}; \
	done > _deps.mk

fmt:
	astyle --style=1tbs -n $(SOURCES) $(HEADERS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include _deps.mk
-include .mk
