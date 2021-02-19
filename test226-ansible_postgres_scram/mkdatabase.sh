#!/bin/sh -eu

database="$1"
owner="$1"
password="$(openssl rand -hex 16)"

psql \
  -v ON_ERROR_STOP=1 \
  -v database="${database}" \
  -v owner="${owner}" \
  -v password="${password}" \
  postgres \
<< END
CREATE DATABASE :"database";
CREATE USER :"owner" WITH PASSWORD :'password';
GRANT ALL PRIVILEGES ON DATABASE :"database" TO :"owner";
END

echo "Database '${database}' created with owner password: ${password}" >&2
