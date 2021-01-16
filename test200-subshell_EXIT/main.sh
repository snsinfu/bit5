#!/bin/sh -eu

(
    echo "entering"

    # Yes, this function is called after "leaving" and before "end".
    cleanup() {
        echo "cleanup"
    }
    trap cleanup EXIT

    echo "leaving"
)

echo "end"
