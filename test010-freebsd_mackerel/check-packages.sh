#!/bin/sh

pkg audit -q > /dev/null
case $? in
1)  echo "vulnerable"
    exit 2  # CRITICAL
    ;;
*)
esac

pkg upgrade -q -Un 2>&1 > /dev/null
case $? in
1)  echo "package updates available"
    exit 1 # WARNING
    ;;
*)
esac
