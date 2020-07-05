#!/bin/sh -eu

wan_interface=em0

jail_release="12.1-RELEASE"
jail_name="pg12"
jail_loopback_ip=127.0.0.11
jail_ip=10.0.2.100


# Install iocage

if [ ! -x /usr/local/bin/iocage ]; then
    pkg update -q
    pkg install -qy py37-iocage
fi

# Enable fdescfs as suggested by iocage

if df -t fdescfs | awk 'NR >= 2 && $6 == "/dev/fd" { exit 1 }'; then
    mount -t fdescfs null /dev/fd
fi

# Fetch base image

if iocage list --base -H | awk -v X="${jail_release}" '$1 == X { exit 1 }'; then
    iocage fetch -r "${jail_release}"
fi

# Create jail

if iocage list -H | awk -v X="${jail_name}" '$2 == X { exit 1 }'; then
    iocage create -n "${jail_name}" -r "${jail_release}"
fi

# Configure jail

iocage set \
    host_hostname="${jail_name}" \
    boot=on \
    assign_localhost=on \
    localhost_ip="${jail_loopback_ip}" \
    ip4_addr="${wan_interface}|${jail_ip}" \
    ip6=disable \
    "${jail_name}"

# Start jail

if iocage list -H | awk -v X="${jail_name}" '$2 == X && $3 == "up" { exit 1 }'; then
    iocage start "${jail_name}"
fi

jail_id="$(iocage list -H | awk -v X="${jail_name}" '$2 == X { print $1 }')"

# Install PostgreSQL

# iocage pkg "${jail_name}" install -qy postgresql12-server

# Configure PostgreSQL

jexec "${jail_id}" tee /etc/rc.conf.d/postgresql << END
postgresql_enable="YES"
postgresql_data="/data/postgresql-12"
END

# Create database

if jexec "${jail_id}" test ! -f /data/postgresql-12/PG_VERSION; then
    jexec "${jail_id}" service postgresql initdb
fi

# Start PostgreSQL

jexec "${jail_id}" service postgresql start
