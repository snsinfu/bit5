CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(OPTFLAGS)

INCLUDES = \
  -isystem ../submodules/github.com/danielaparker/jsoncons/include

OPTFLAGS = \
  -O2

ARTIFACTS = \
  main


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)
