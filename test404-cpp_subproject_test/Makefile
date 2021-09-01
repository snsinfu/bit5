CXXFLAGS = \
  $(STDFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES)

STDFLAGS = \
  -std=c++20 \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion

OPTFLAGS = \
  -O2

MAIN = \
  main

SOURCES = \
  $(wildcard *.cpp) \
  $(wildcard simulator/*.cpp)

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
