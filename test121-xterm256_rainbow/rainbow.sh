#!/bin/sh

rainbow16="196 208 220 226 154 46 47 49 51 39 27 21 129 201 200 198"

show_rainbow() {
    start=${1-0}
    end=$((start + 16))
    phase=0
    for code in ${rainbow16} ${rainbow16}; do
        if [ ${phase} -ge ${start} -a ${phase} -lt ${end} ]; then
            printf "\x1b[48;5;%dm " ${code}
        fi
        phase=$((phase + 1))
    done
    printf "\x1b[0m\n"
}

i=0
while [ ${i} -lt 100 ]; do
    printf "%2d " ${i}
    show_rainbow $((i % 16))
    sleep 0.1
    printf "\x1b[1A\x1b[2K"
    i=$((i + 1))
done
