PRODUCTS = \
  hash-aaa.txt \
  hash-bbb.txt \
  hash-ccc-1.txt

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

hash-%.txt: source-%.dat
	sha256sum $< > $@
