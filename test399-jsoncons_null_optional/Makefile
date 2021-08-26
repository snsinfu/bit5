CXXFLAGS = \
  -std=c++20 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  $(INCLUDES) \
  -O2

INCLUDES = \
  -isystem ../submodules/github.com/danielaparker/jsoncons/include

.PHONY: run clean

run: main
	./main

clean:
	rm -f main
