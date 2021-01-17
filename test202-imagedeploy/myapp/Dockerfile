FROM golang:1.15 AS builder
WORKDIR /srv
COPY . .
RUN export CGO_ENABLED=0 \
 && go get -d ./... \
 && go build

FROM scratch
WORKDIR /srv
COPY --from=builder /srv/myapp /srv/myapp
ENTRYPOINT ["/srv/myapp"]
