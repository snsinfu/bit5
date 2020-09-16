MPIFC = mpif90
MPIFFLAGS =

.PHONY: run clean

run: main
	mpirun -np 2 ./main

clean:
	rm -f main

main: main.f95
	$(MPIFC) $(MPIFFLAGS) -o $@ $< $(LDFLAGS)
