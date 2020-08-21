PRODUCTS = \
  main.png

DOTFLAGS = \
  $(USE_SANS)

USE_SANS = \
  -Gfontname=helvetica \
  -Nfontname=helvetica \
  -Efontname=helvetica \
  -Gfontsize=10 \
  -Nfontsize=10 \
  -Efontsize=10

USE_ORTHO = \
  -Gsplines=ortho


.PHONY: all clean
.SUFFIXES: .dot .png

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.dot.png:
	dot $(DOTFLAGS) -Tpng -o$@ $^
