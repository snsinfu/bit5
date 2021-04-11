.PHONY: all

all: network.sat network.png
	@:

network.cnf network.dot: generate_network.ts
	deno run --allow-read=. --allow-write=. $<

network.png: network.dot
	dot -Tpng -o$@ $<

network.sat: network.cnf
	z3 $< > $@
