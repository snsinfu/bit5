MPIFC = mpif90
MPIFFLAGS =

OBJECTS = main.o rex.o
MODS = rex.mod
PRODUCT = main
ARTIFACTS = $(OBJECTS) $(MODS) $(PRODUCT)

.PHONY: run clean
.SUFFIXES: .f95

run: main
	mpirun -np 2 ./main

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(MPIFC) $(MPIFFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.f95.o:
	$(MPIFC) $(MPIFFLAGS) -c -o $@ $<

main.o: rex.o
