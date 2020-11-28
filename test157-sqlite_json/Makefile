CFLAGS = \
  -std=c99 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2

LDFLAGS = \
  -lm \
  -lsqlite3

.PHONY: run clean

run: main
	rm -f _output.db
	./main

clean:
	rm -f main _output.db
