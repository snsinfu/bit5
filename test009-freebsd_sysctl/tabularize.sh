#!/bin/sh -eu

TAB=$(printf "\t")

join -t "${TAB}" _sysctl_t.txt _sysctl_v.txt |
join -t "${TAB}" -             _sysctl_d.txt |
awk '
BEGIN {
    FS = "\t"

    print "# OIDs"
    print ""
    print "| oid | type | value | description |"
    print "|-----|------|-------|-------------|"
}
function escape(s) {
    gsub(/</, "\\&lt;", s)
    gsub(/>/, "\\&gt;", s)
    return s
}
{
    oid = $1
    type = $2
    value = $3
    description = $4
    value = escape(value)
    description = escape(description)
    print "| " oid " | " type " | " value " | " description " |"
}'
