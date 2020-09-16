FC = gfortran
FFLAGS = -std=f95 -Wpedantic -Wall -Wextra

.PHONY: run clean

run: main
	./main

clean:
	rm -f main

main: main.f90
	$(FC) $(FFLAGS) -o $@ $< $(LDFLAGS)
