CXXFLAGS = \
  -std=c++20 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -isystem ../submodules/github.com/snsinfu/h5/include \
  -isystem ../submodules/github.com/snsinfu/micromd/include \
  -O2

LDFLAGS = \
  -lhdf5


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
