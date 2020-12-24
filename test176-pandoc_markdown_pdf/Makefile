PAN = pandoc

PANFLAGS = \
  --filter pandoc-crossref \
  --citeproc \
  --defaults defaults.yaml

PRODUCTS = main.pdf


.PHONY: all clean
.SUFFIXES: .md .pdf

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.md.pdf:
	$(PAN) $(PANFLAGS) -f markdown -t pdf -o $@ $<
