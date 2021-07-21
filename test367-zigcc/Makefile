CXX = zig c++
CXXFLAGS = -std=c++20 -Wpedantic -Wall -Wextra -O2 $(CXXFLAGS_FIX)

# Workaround for error: MissingMainEntryPoint
# https://github.com/ziglang/zig/issues/8531
# https://github.com/ziglang/zig/issues/8552
CXXFLAGS_FIX = -fno-lto


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
