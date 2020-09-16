module rex
    implicit none

    public :: rex_init
    public :: rex_id
    public :: rex_try_exchange
    public :: rex_finalize

    private

    include "mpif.h"

    ! MPI message tag used for replica exchange messages. This is an arbitrary
    ! number that is not used for any other messages.
    integer, parameter :: TAG_EXCHANGE = 1

    ! Maximum number of expected replicas (MPI processes).
    integer, parameter :: MAX_REPLICAS = 256

    ! Temperature assigned to each replica.
    real :: temperature_list(MAX_REPLICAS)

    ! Number of MPI processes.
    integer :: processes

    ! Fixed MPI rank of this process.
    integer :: self_rank

    ! Replica ID currently owned by this process. This nunmber changes as
    ! replica exchange simulation progresses.
    integer :: self_id

    ! Number of replica exchange attempts so far.
    integer :: exchange_attempts

contains

    ! Initializes MPI and replicas. This subroutine must be called before any
    ! other subroutines in this module.
    subroutine rex_init(temps)
        real temps(:)
        integer err

        call mpi_init(err)
        call mpi_comm_size(MPI_COMM_WORLD, processes, err)
        call mpi_comm_rank(MPI_COMM_WORLD, self_rank, err)

        ! Set the temperature of each replica as specified. XXX: The caller
        ! needs to provide temperature values without knowing the number of
        ! replicas. Bad API design!
        temperature_list(:processes) = temps(:processes)

        ! Set initial replica ID of this process (which will be altered upon
        ! subsequent replica exchange events). Note that the MPI rank is
        ! zero-based. So, +1 to make replica ID one-based.
        self_id = self_rank + 1
        exchange_attempts = 0

        return
    end subroutine

    ! Gets the current replica ID owned by this process.
    subroutine rex_id(id)
        integer id
        id = self_id
        return
    end subroutine

    !
    subroutine rex_try_exchange(energy, ok)
        real energy
        logical ok
        integer peer_rank
        integer last_id

        ! Attempt
        !
        ! even      [0 1] [2 3] [4 ...
        ! odd       0 [1 2] [3 4] ...
        !
        !
        !
        if (mod(exchange_attempts, 2) == 0) then
            if (mod(self_rank, 2) == 0) then
                peer_rank = self_rank + 1
            else
                peer_rank = self_rank - 1
            end if
        else
            if (mod(self_rank, 2) == 0) then
                peer_rank = self_rank - 1
            else
                peer_rank = self_rank + 1
            end if
        end if

        last_id = self_id

        if (peer_rank >= 0 .and. peer_rank < processes) then
            if (self_rank > peer_rank) then
                call rex_try_exchange_as_master(energy, peer_rank)
            else
                call rex_try_exchange_as_slave(energy, peer_rank)
            end if
        end if

        exchange_attempts = exchange_attempts + 1
        ok = self_id /= last_id

        return
    end subroutine

    subroutine rex_try_exchange_as_master(self_energy, peer_rank)
        real self_energy, peer_energy
        real self_temp, peer_temp
        integer peer_rank
        real message(2)
        integer status_(MPI_STATUS_SIZE)
        integer err
        integer peer_id, tmp_id
        real u_rand, exp_rand

        call mpi_recv(message, 2, MPI_REAL, peer_rank, TAG_EXCHANGE, MPI_COMM_WORLD, status_, err)
        peer_id = int(message(1))
        peer_energy = message(2)

        self_temp = temperature_list(self_id)
        peer_temp = temperature_list(peer_id)

        call random_number(u_rand)
        exp_rand = -log(1 - u_rand)

        if (exp_rand >= (1 / self_temp - 1 / peer_temp) * (peer_energy - self_energy)) then
            tmp_id = self_id
            self_id = peer_id
            peer_id = tmp_id
        end if

        message(1) = real(peer_id)
        message(2) = peer_energy
        call mpi_send(message, 2, MPI_REAL, peer_rank, TAG_EXCHANGE, MPI_COMM_WORLD, err)

        return
    end subroutine

    subroutine rex_try_exchange_as_slave(self_energy, peer_rank)
        real self_energy
        integer peer_rank
        real message(2)
        integer status_(MPI_STATUS_SIZE)
        integer err

        message(1) = real(self_id)
        message(2) = self_energy
        call mpi_send(message, 2, MPI_REAL, peer_rank, TAG_EXCHANGE, MPI_COMM_WORLD, err)
        call mpi_recv(message, 2, MPI_REAL, peer_rank, TAG_EXCHANGE, MPI_COMM_WORLD, status_, err)
        self_id = int(message(1))

        return
    end subroutine

    ! Finalizes MPI. This subroutine must be called before the program ends.
    subroutine rex_finalize()
        integer err

        call mpi_finalize(err)

        return
    end subroutine
end module
