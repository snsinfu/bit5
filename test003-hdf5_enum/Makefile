CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2

LDFLAGS = \
  -lhdf5


.PHONY: inspect clean

inspect: _output.h5
	h5dump $^

clean:
	rm -f main _output.h5

_output.h5: main
	./main

-include .mk
