zmodload zsh/sched

RPROMPT="[%*]"

update_prompt() {
    zle && {
        zle reset-prompt
    }
    sched +1 update_prompt
}
update_prompt
