CFLAGS = \
  -std=c99 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2 \
  -march=native

.PHONY: run clean fmt

run: main
	@./main

clean:
	rm -f main

fmt:
	astyle --style=1tbs -n main.c
