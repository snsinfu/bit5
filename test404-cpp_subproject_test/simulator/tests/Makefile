CXXFLAGS = \
  $(STDFLAGS) \
  $(DBGFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES)

STDFLAGS = \
  -std=c++20 \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion

DBGFLAGS = \
  -g \
  -fsanitize=address

OPTFLAGS = \
  -Og

INCLUDES = \
  -isystem ../../../submodules/github.com/onqtam/doctest

MAIN = \
  main

SOURCES = \
  $(wildcard *.cpp) \
  $(wildcard ../*.cpp)

OBJECTS = \
  $(SOURCES:.cpp=.o)

ARTIFACTS = \
  $(MAIN) \
  $(OBJECTS)


.PHONY: run clean

run: $(MAIN)
	@./$(MAIN)

clean:
	rm -f $(ARTIFACTS)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
