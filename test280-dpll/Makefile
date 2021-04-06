.PHONY: run clean

run: main
	@./main

clean:
	rm -f main

main: main.rs
	rustc -o $@ $<
