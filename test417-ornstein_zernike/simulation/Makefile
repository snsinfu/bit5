CXXFLAGS = \
  -std=c++20 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2 \
  -march=native \
  -isystem ../../submodules/github.com/snsinfu/h5/include \
  -isystem ../../submodules/github.com/snsinfu/micromd/include

LDFLAGS = \
  -lhdf5


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
