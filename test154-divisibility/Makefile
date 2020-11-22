CFLAGS = -std=c99 -Wpedantic -Wall -Wextra -O2

.PHONY: all clean test

all: main
	@:

clean:
	rm -f main

test: main
	./main < testcase_1.txt
	./main < testcase_2.txt
	./main < testcase_3.txt
