program main
    character(16) filename
    integer step

    do step = 0, 1000, 100
        write(filename, '(a, i5.5, a)') 'output-', step, '.dat'
        print *, filename
    end do
end program main
