CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2


.PHONY: all run clean fmt

all: main
	@:

run: main
	@./main

clean:
	rm -f main

fmt:
	astyle -style=1tbs -n main.cc

-include .mk
