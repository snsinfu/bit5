#!/bin/sh

${SHELL:-sh} trap.sh

# This code does not run when `trap.sh` exited due to a signal (which is a
# correct behavior; the signal is propagated up to this script).
echo "Exit code: $?"
