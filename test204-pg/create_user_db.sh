#!/bin/sh -eu

name="$1"
pass=$(openssl rand -hex 16)

echo "Creating database and user ${name}..."

psql -h localhost -p 5432 -d postgres -U postgres -w -q << END
CREATE USER ${name} WITH PASSWORD '${pass}';
CREATE DATABASE ${name};
GRANT ALL PRIVILEGES ON DATABASE ${name} TO ${name};
END

echo "OK"

echo "${pass}" > ~/${name}.pass
chmod 0600 ~/${name}.pass

echo "Password saved to ~/${name}.pass"
