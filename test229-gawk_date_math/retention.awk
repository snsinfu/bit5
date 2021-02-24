#!/usr/bin/env gawk -f

function join(array, start, end, sep, i, result) {
    for (i = start; i <= end; i++) {
        if (i == start) {
            result = array[i]
        } else {
            result = result sep array[i]
        }
    }
    return result
}

BEGIN {
    UTC = 1
    DAY = 60 * 60 * 24
    TTL = 7 * DAY

    # now = systime()
    now = 1614187988
}

{
    timetag = $1
    expired = 0

    pos = match(timetag, /([0-9]+)([0-9]{2})([0-9]{2})T([0-9]{2})([0-9]{2})([0-9]{2})/, timerec)
    if (pos == 1) {
        time = mktime(join(timerec, 1, 6, " "), UTC)
        expire = time + TTL

        if (now >= expire) {
            expired = 1
        }
    }

    if (expired) {
        print "expired!", timetag
    } else {
        print "retained", timetag
    }
}
