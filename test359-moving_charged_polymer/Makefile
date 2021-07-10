CXXFLAGS = \
  $(WARNFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES)

WARNFLAGS = \
  -std=c++20 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-compare \
  -Wshadow

OPTFLAGS = \
  -O2 \
  -march=native

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/micromd/include

PRODUCTS = \
  main


.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)
