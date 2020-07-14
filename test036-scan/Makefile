RUSTC = rustc
RUSTFLAGS = -O

.PHONY: all clean

all: main
	@:

clean:
	rm -f main

main: main.rs
	$(RUSTC) $(RUSTFLAGS) -o $@ $<
