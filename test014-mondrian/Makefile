WXLIBS = core,base

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  $$(wx-config --cxxflags | sed "s/-I/-isystem/g") \
  -isystem/usr/include/eigen3

LDFLAGS = \
  $$(wx-config --libs $(WXLIBS))

SOURCES = \
  $(wildcard *.cpp)

HEADERS = \
  $(wildcard *.h)

OBJECTS = \
  $(SOURCES:.cpp=.o)

ARTIFACTS = \
  main \
  _depends.mk \
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
	done > _depends.mk

fmt:
	astyle --style=1tbs -n $(SOURCES) $(HEADERS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include: _depends.mk
