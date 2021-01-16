#!/bin/sh

(
    tmpdir="$(mktemp -d)"

    cleanup() {
        echo "Cleaning up..."
        rm -rf "${tmpdir}"  # OK, this works.
    }
    trap cleanup EXIT

    cd "${tmpdir}"
    echo "Working in ${tmpdir}..."

    echo "Do something"
)

echo "OK"
