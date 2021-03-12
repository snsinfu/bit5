#!/bin/sh -eu

date > date.txt
kim build -t localhost/whoami/web:latest .

date > date.txt
kim build -t localhost/whoami/web:latest .
