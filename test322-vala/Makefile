VALAC = valac
VALAFLAGS = --pkg gtk+-2.0

.PHONY: run clean

run: main
	@./main

clean:
	rm -f main

main: main.vala
	$(VALAC) $(VALAFLAGS) -o $@ $^
