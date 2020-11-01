rainbow() {
    local i=$1
    local palette=( 197 220 82 49 33 129 )
    local size=${#palette}

    # Normalize to non-negative index by cycling.
    i=$(( i % size + size ))

    echo ${palette[(i - 1) % size + 1]}
}

rainbow_path() {
    local path=$1
    local phase=$2
    local components=
    local directory=
    local color=
    local i=
    local decorated_path=

    components=( ${(s:/:)path}n )

    case ${path} in
    /)  components=( "" )
        ;;
    ~*) components=( ${components[2,#components]} )
        color=$(rainbow $((1 - phase)))
        decorated_path="%F{${color}}~"
        ;;
    esac

    for ((i = 1; i <= ${#components}; i++)); do
        directory=${components[i]}
        color=$(rainbow $((i - phase)))
        decorated_path+="%F{#888}/"
        decorated_path+="%F{${color}}${directory}"
    done

    echo ${decorated_path}
}

make_prompt() {
    local phase=$1
    local host=${(%)$(echo "%m")}
    local user=${(%)$(echo "%n")}
    local path=${(%)$(echo "%~")}
    local prompt_text=

    prompt_text+="%F{red}${host:(-6)}%F{reset}:${user}"
    prompt_text+=$(rainbow_path ${path} ${phase})
    prompt_text+=" %F{reset}%%"
    prompt_text+="%F{reset} "

    echo ${prompt_text}
}

set prompt_subst

prompt_offset=0
PROMPT='$(make_prompt ${prompt_offset})'


zmodload zsh/sched

update_prompt() {
    zle && {
        zle reset-prompt
        : $(( prompt_offset += 1))
    }
    sched +1 update_prompt
}
update_prompt
