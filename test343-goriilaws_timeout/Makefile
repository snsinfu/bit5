.PHONY: all clean

all: client server
	@:

clean:
	rm -f client server

client: client.go
	go build -o $@ $<

server: server.go
	go build -o $@ $<
