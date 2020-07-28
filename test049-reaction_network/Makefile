RUSTC = rustc

.PHONY: all clean

all: _trajectory.dat
	@:

clean:
	rm -f main _trajectory.dat

_trajectory.dat: main
	./main > $@

main: main.rs
	$(RUSTC) main.rs
