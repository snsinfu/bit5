CXXFLAGS = \
  $(LANGFLAGS) \
  $(OPTFLAGS)

LANGFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \

OPTFLAGS = \
  -O2

LIBS = \
  -lhdf5


.PHONY: all clean

all: main
	@:

clean:
	rm -f main _output.h5

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
