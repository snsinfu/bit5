module sfc
    implicit none

    public :: sfc_seed
    public :: sfc_rand_bits
    public :: sfc_rand_uniform

private
    ! SFC64 pseudorandom number generator state.
    integer(8) :: sfc_state(4)

contains
    subroutine sfc_seed(seed)
        integer(8), intent(in) :: seed
        integer(8) :: discard
        integer :: i

        sfc_state(1:3) = seed
        sfc_state(4) = 0

        do i = 1, 12
            call sfc_rand_bits(discard)
        end do

        return
    end subroutine

    subroutine sfc_rand_bits(val)
        integer(8), intent(out) :: val
        integer(8) :: a, b, c, x

        a = sfc_state(1)
        b = sfc_state(2)
        c = sfc_state(3)
        x = sfc_state(4)

        val = a + b + x
        x = x + 1
        a = xor(b, rshift(b, 11))
        b = c + lshift(c, 3)
        c = val + or(lshift(c, 24), rshift(c, 64 - 24))

        sfc_state = (/ a, b, c, x /)

        return
    end subroutine

    subroutine sfc_rand_uniform(val)
        real, intent(out) :: val
        integer(8) :: bits

        call sfc_rand_bits(bits)
        val = 0.5 + real(bits) / (2.0 ** 64)

        return
    end subroutine
end module
