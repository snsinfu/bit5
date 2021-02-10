dotenv() {
    if [ $# -eq 0 ]; then
        [ -f .env.gpg ] && set -- .env.gpg "$@"
        [ -f .env     ] && set -- .env     "$@"
    fi

    while [ $# -gt 0 ]; do
        case "$1" in
        *.gpg)
            eval "$(gpg --quiet --decrypt --yes "$1")"
            ;;
        */*)
            . "$1"
            ;;
        *)
            . "./$1"
        esac
        shift
    done
}
