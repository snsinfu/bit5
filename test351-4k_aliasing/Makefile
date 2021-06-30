CXXFLAGS = \
  $(STANDARDS) \
  $(INCLUDES) \
  $(OPTIMIZATIONS)

STANDARDS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion

OPTIMIZATIONS = \
  -O2

PRODUCTS = \
  main


.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)
