program main
    implicit none

    include "mpif.h"

    integer comm_size, comm_rank
    integer err

    call mpi_init(err)
    call mpi_comm_size(MPI_COMM_WORLD, comm_size, err)
    call mpi_comm_rank(MPI_COMM_WORLD, comm_rank, err)

    print "(i1,a,i1)", comm_rank, "/", comm_size

    call mpi_finalize(err)
end program
