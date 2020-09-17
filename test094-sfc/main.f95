program main
    use sfc

    implicit none

    integer(8) :: seed
    integer :: step
    real :: u

    seed = 1
    call sfc_seed(seed)

    do step = 1, 1000000
        call sfc_rand_uniform(u)
        print "(f9.6)", u
    end do
end program
