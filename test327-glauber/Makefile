CXXFLAGS = \
  -std=c++2a \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  -march=native \
  $(INCLUDES)

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/h5/include

LIBS = \
  -lhdf5


.PHONY: all clean

all: main
	@:

clean:
	rm -f main _output.h5

main: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
