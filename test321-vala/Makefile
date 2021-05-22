VALAC = valac

.PHONY: run clean

run: main
	@./main

clean:
	rm -f main

main: main.vala
	$(VALAC) -o $@ $^
