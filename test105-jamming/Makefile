CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wno-c99-extensions \
  $(OPTFLAGS) \
  $(INCLUDES)

OPTFLAGS = \
  -O2 \
  -march=native

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/h5/include \
  -isystem ../submodules/github.com/snsinfu/micromd/include

LIBRARIES = \
  -lhdf5

ARTIFACTS = \
  main

.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBRARIES)

main: format.hpp power_potential.hpp
