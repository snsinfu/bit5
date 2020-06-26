CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -O2

SOURCES = \
  main.cc

BACKUPS = \
  $(addsuffix .orig, $(SOURCES))

ARTIFACTS = \
  $(PRODUCT) \
  $(BACKUPS)

PRODUCT = \
  main


.PHONY: all style clean

all: $(PRODUCT)
	@:

style:
	astyle --options=astylerc $(SOURCES)

clean:
	rm -f $(ARTIFACTS)
