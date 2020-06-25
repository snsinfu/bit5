CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -W \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2


.PHONY: run clean format

run: main
	./main

clean:
	rm -f main

format:
	clang-format --style=WebKit -i main.cc

-include .mk
