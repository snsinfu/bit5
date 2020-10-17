zmodload zsh/sched

rainbow_palette=(
    196 202 208 214 220 226 226 190 154 82 46 46 47 48 49
    50 51 45 39 33 27 21 21 57 129 165 201 201 200 199 198 197
)
rainbow_offset=0

rainbow_color:inc() {
    local n=${#rainbow_palette}
    echo -n ${rainbow_palette[1 + ($1 + rainbow_offset - 1) % n]}
}

rainbow_color:dec() {
    local n=${#rainbow_palette}
    local i=$((n - rainbow_offset % n + $1))
    echo -n ${rainbow_palette[1 + (i - 1) % n]}
}

rainbow_prompt() {
    local text="%m:%n %~"
    local chars=( ${(s::)${(%)text}} )
    local deco_prompt=
    local i
    local n=${#rainbow_palette}

    for ((i = n / 2; i <= n; i++)); do
        deco_prompt+="%K{$(rainbow_color:dec ${i})} "
    done

    deco_prompt+="%K{reset} "

    for ((i = 1; i <= ${#chars}; i++)); do
        deco_prompt+="%F{$(rainbow_color:dec ${i})}"
        deco_prompt+=${chars[i]}
    done

    echo -n "${deco_prompt}%F{reset} %F{cyan}%%%F{reset} "
}

PROMPT='$(rainbow_prompt)'

update_prompt() {
    zle && {
        zle reset-prompt
        : $((rainbow_offset += 1))
    }
    sched +1 update_prompt
}
update_prompt
