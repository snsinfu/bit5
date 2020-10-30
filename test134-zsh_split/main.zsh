#!/usr/bin/env zsh
set -eu

path=$PWD

components=( ${(s:/:)path} )
decorated_path=

palette=(
    197 220 82 49 33 129
)

decorated_path+="%K{#000}"
for ((i = 1; i <= ${#components}; i++)); do
    dir=${components[i]}
    n=${#palette}
    color=${palette[i % n + 1]}

    decorated_path+="%F{#888}/"
    decorated_path+="%F{${color}}${dir}"
done
decorated_path+="%K{reset}%F{reset}"

echo ${(%)decorated_path}
