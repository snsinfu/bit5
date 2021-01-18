#!/bin/sh -eu

set -eu

# Manual propagation of exit status and signal on cleanup.
#
# Trapping a signal stops propagation of the signal to the parent process. So,
# the script needs to trap each signal individually and kill itself with the
# trapped signal after cleanup.
#
# Also, trapping EXIT masks the exit status of the failed command when -e shell
# option is set. Hence, the script needs to capture the exit status and exit
# with the captured status.

cleanup() {
    # Cleanup code should proceed anyway on failure. Like non-throwing
    # destructor in C++.
    set +e
    echo "Clean up"
    sh -c "exit 10"  # OK
}

cleanup_exit() {
    code=$?
    echo "Status ${code}"

    cleanup

    exit ${code}
}

cleanup_signal() {
    sig=$1
    echo "Signal ${sig}"

    cleanup

    # Need to untrap signals. Also EXIT needs be untrapped because bash calls
    # EXIT handler also on signal exit.
    trap - EXIT INT TERM
    kill -${sig} $$
}

# Let cleanup procedure run when the script exits with success, failure or a
# signal (only INT and TERM). A script should also trap PIPE if the stdout
# is expected to be piped to another process (because it may fail and break
# the pipe). You may also want to trap HUP, USR1 and USR2 depennding on what
# is expected. I think scripts should not trap QUIT because the QUIT signal
# should cause an immediate, dirty exit with core dump.

trap cleanup_exit EXIT
trap "cleanup_signal INT" INT
trap "cleanup_signal TERM" TERM


echo "Press enter to exit with success, Ctrl-D to exit with error, or"
echo "Ctrl-C or send SIGTERM to pid $$ to exit with a signal"
echo "---"

read _
