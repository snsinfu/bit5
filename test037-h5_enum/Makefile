CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  $(INCLUDES) \
  $(OPTFLAGS)

INCLUDES = \
  -isystem../submodules/github.com/snsinfu/h5/include

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lhdf5


.PHONY: inspect clean

all: main
	@:

clean:
	rm -f main

-include .mk
