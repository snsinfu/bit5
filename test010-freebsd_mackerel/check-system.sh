#!/bin/sh

cd /var/db/freebsd-update

freebsd-update updatesready 2>&1 > /dev/null
case $? in
0)  echo "system updates available"
    exit 2 # CRITICAL
    ;;
2)  exit 0 # OK
esac
