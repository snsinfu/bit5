FC = gfortran
FFLAGS =

OBJECTS = main.o sfc.o
ARTIFACTS = main sfc.mod $(OBJECTS)

.PHONY: run clean
.SUFFIXES: .f95

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(FC) $(FFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.f95.o:
	$(FC) $(FFLAGS) -c -o $@ $<

main.o: sfc.o
