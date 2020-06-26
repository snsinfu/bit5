CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2

SOURCES = \
  main.cc


.PHONY: all format clean

all: main
	@:

format:
	astyle --style=1tbs -n $(SOURCES)

clean:
	rm -f main
