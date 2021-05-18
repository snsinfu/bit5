CXXFLAGS = \
  -std=c++2a \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(DBGFLAGS) \
  $(OPTFLAGS)

OPTFLAGS = \
  -O2 \
  -march=native

ARTIFACTS = \
  $(PRODUCTS) \
  $(OBJECTS)

SOURCES = \
  $(wildcard *.cpp)

OBJECTS = \
  $(SOURCES:.cpp=.o)

PRODUCTS = \
  _depends.mk \
  main

LIBS = \
  -lhdf5

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/h5/include \
  -isystem ../submodules/github.com/snsinfu/micromd@variant-2d/include


.PHONY: all clean
.SUFFIXES: .cpp

all: $(PRODUCTS)
	@:

clean:
	rm -f $(ARTIFACTS)

_depends.mk: $(SOURCES)
	for src in $(SOURCES); do \
	  $(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM -MF- -MT $${src%.*}.o $${src}; \
	done

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

-include _depends.mk
