#!/bin/sh

while read oldrev newrev refname; do
    echo "${refname}: ${oldrev} -> ${newrev}"
done
