#!/usr/bin/zsh

zmodload zsh/net/socket

sockname="_channel.sock"
listenfd=20

server() {
    trap "rm -f ${sockname}" EXIT
    zsocket -l -d ${listenfd} ${sockname}
    zsocket -a ${listenfd}
    socket=${REPLY}

    sum=0
    read count <& ${socket}
    for ((i = 0; i < ${count}; i++)); do
        read x <& ${socket}
        : $((sum += x))
    done
    echo "${sum}" >& ${socket}
}

client() {
    zsocket ${sockname}
    socket=${REPLY}
    data=(7 2 5 1 3 6 4)

    echo "${#data}" >& ${socket}
    for x in ${data}; do
        echo "${x}" >& ${socket}
    done
    read sum <& ${socket}

    echo "Response: ${sum}"
}

server &
sleep 1
client
