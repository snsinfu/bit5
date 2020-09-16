program main
    use rex

    real temperature_list(10)
    real energy
    logical ok
    integer replica_id
    integer i
    character(13) filename

    ! Geometric progression.
    temperature_list = (/ 1.00, 1.05, 1.10, 1.16, 1.22, 1.28, 1.34, 1.41, 1.48, 1.55 /)

    call rex_init(temperature_list)
    call rex_id(replica_id)

    do i = 1, 10
        call random_number(energy)
        call rex_try_exchange(energy, ok)
        call rex_id(replica_id)

        write(filename, "(a,i2.2,a)") "output-", replica_id, ".dat"
        print *, i, filename
    end do

    call rex_finalize()
end program
