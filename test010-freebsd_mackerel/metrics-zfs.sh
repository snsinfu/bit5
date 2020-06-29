#!/bin/sh

timestamp=$(date +%s)

arc_size=$(sysctl -n kstat.zfs.misc.arcstats.size)

printf "%s\t%s\t%s\n" zfs.arcusage.size ${arc_size} ${timestamp}
