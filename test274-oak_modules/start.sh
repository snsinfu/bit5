#!/bin/sh -eu

set -a
. ./.env
set +a

exec deno run \
  --allow-env \
  --allow-net \
  --allow-read=static \
  main.ts
