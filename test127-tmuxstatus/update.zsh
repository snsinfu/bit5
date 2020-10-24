#!/usr/bin/zsh

rainbow_palette=(
    196 202 208 214 220 226 226 190 154 82 46 46 47 48 49
    50 51 45 39 33 27 21 21 57 129 165 201 201 200 199 198 197
)

rainbow() {
    local n=${#rainbow_palette}
    local i=$1
    if (( i < 0 )); then
        i=$(( n - (-i % n) ))
    fi
    i=$(( i % n ))
    echo "${rainbow_palette[i + 1]}"
}

print_status() {
    local text=" %D{%F (%a) %T} "
    local phase
    local deco_text
    local chars
    local i
    local fg_color

    phase="%D{%s}"
    phase=${(%)phase}

    chars=( ${(s::)${(%)text}} )
    deco_text+="#[bg=colour16,bold]"

    for ((i = 1; i <= ${#chars}; i++)); do
        fg_color=$(rainbow $(( i + phase )))
        deco_text+="#[fg=colour${fg_color}]"
        deco_text+=${chars[i]}
    done

    echo "${deco_text}"
}


# This periodically changes tmux status from outside. In "real" configuration
# you should create a script that outputs the result of print_status to stdout
# and set status-right to "#(/path/to/that/script)".

while :; do
    sleep 1
    tmux set -g status-right "$(print_status)"
done
