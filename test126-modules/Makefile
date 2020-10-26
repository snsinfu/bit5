CXX = clang++-11

CXXFLAGS = \
  -std=c++2a \
  -stdlib=libc++ \
  -fimplicit-modules \
  -fimplicit-module-maps \
  -fprebuilt-module-path=.

.PHONY: all clean
.SUFFIXES: .cppm .pcm

all: main
	@:

clean:
	rm -f main main.o math.pcm io.pcm

main: main.o math.pcm io.pcm
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: math.pcm io.pcm

.cppm.pcm:
	$(CXX) $(CXXFLAGS) --precompile -o $@ $<
