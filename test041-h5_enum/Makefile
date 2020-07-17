CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  $(INCLUDES) \
  $(OPTFLAGS)

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
