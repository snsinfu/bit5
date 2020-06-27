CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  $(OPTFLAGS)

OPTFLAGS = \
  -march=native \
  -mtune=native \
  -O2


.PHONY: all run clean fmt

all: main
	@:

run: main
	./main 9 3

clean:
	rm -f main

fmt:
	astyle -style=1tbs -n main.cc

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main: buffer.hpp matrix.hpp

-include .mk
