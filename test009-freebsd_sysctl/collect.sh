#!/bin/sh -eu

normalize() {
    tr -d '\r' |
    awk 'BEGIN { FS = ": "; OFS = "\t" } NF > 1 { print $1, $2 }' |
    sort -k 1,1
}

vagrant ssh -c 'sudo sysctl -Wao -d' | normalize > _sysctl_d.txt
vagrant ssh -c 'sudo sysctl -Wao -t' | normalize > _sysctl_t.txt
vagrant ssh -c 'sudo sysctl -Wao'    | normalize > _sysctl_v.txt
