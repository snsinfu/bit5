CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  -march=native

.PHONY: all clean plot

all: main
	@:

clean:
	rm -f main _output.dat _output.png

plot: _output.png
	@:

_output.dat: main
	./main > $@

_output.png: _output.dat
	python plot.py
