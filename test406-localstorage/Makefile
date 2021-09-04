TSC = tsc
TSCFLAGS = --target es6 --strict

PRODUCTS = \
  app.js

ARTIFACTS = \
  $(PRODUCTS)


.PHONY: all clean serve
.SUFFIXES: .ts .js

all: $(PRODUCTS)
	@:

clean:
	rm -f $(ARTIFACTS)

serve: $(PRODUCTS)
	caddy

.ts.js:
	$(TSC) $(TSCFLAGS) $<
