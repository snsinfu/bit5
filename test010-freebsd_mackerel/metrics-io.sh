#!/bin/sh

timestamp=$(date +%s)

open_files=$(sysctl -n kern.openfiles)
connected_sockets=$(sockstat -q46c | wc -l)
listening_sockets=$(sockstat -q46l | wc -l)

printf "%s\t%s\t%s\n" io.files.open        ${open_files}        ${timestamp}
printf "%s\t%s\t%s\n" io.sockets.connected ${connected_sockets} ${timestamp}
printf "%s\t%s\t%s\n" io.sockets.listening ${listening_sockets} ${timestamp}
