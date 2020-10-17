zmodload zsh/sched

rainbow_palette=(196 214 226 154 46 49 51 39 21 129 201 199)
rainbow_offset=0

rainbow_color() {
    local n=${#rainbow_palette}
    local i=$((n - rainbow_offset % n + $1))
    echo -n ${rainbow_palette[1 + (i - 1) % n]}
}

rainbow_prompt() {
    local text="%m:%n %~"
    local chars=( ${(s::)${(%)text}} )
    local deco_prompt=
    local i

    for ((i = 1; i <= ${#rainbow_palette}; i++)); do
        deco_prompt+="%K{$(rainbow_color ${i})} "
    done

    deco_prompt+="%K{reset} "

    for ((i = 1; i <= ${#chars}; i++)); do
        deco_prompt+="%F{$(rainbow_color ${i})}"
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
