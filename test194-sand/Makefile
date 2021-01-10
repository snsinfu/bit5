CXXFLAGS = \
  -std=c++2a \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(OPTFLAGS)

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/micromd@variant-2d/include \
  -isystem ../submodules/github.com/snsinfu/h5/include

LIBS = \
  -lhdf5

OPTFLAGS = \
  -O2 \
  -march=native

ARTIFACTS = \
  main \
  _output.h5


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: main.cc data.hpp forcefield.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)
