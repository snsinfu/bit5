FROM hayd/alpine-deno:1.8.0

USER deno
WORKDIR /srv

COPY deps.ts .
RUN deno cache deps.ts

COPY . .
RUN deno cache main.ts

CMD ["run", "--allow-env", "--allow-net", "--allow-read", "main.ts"]
