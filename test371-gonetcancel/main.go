package main

import (
	"context"
	"errors"
	"log"
	"net"
	"os"
	"os/signal"
)

const (
	remoteAddress = "127.0.0.1:9999"
	bufferSize    = 1024
)

func main() {
	ctx, cancel := context.WithCancel(context.Background())

	sigCh := make(chan os.Signal)
	signal.Notify(sigCh, os.Interrupt)

	go func() {
		<-sigCh
		cancel()
	}()

	if err := run(ctx); err != nil {
		log.Printf("error: %s", err)
		os.Exit(1)
	}
}

func run(ctx context.Context) error {
	dialer := net.Dialer{}
	conn, err := dialer.DialContext(ctx, "tcp", remoteAddress)
	if err != nil {
		return err
	}
	defer conn.Close()

	// Context cancellation only affects Dial call. Read won't be cancelled.
	// So, we need to manually force-close connection to cancel reading from a
	// network connection.

	end := make(chan struct{})
	defer close(end)
	go func() {
		select {
		case <-ctx.Done():
			conn.Close()
		case <-end:
		}
	}()

	log.Print("reading from remote")

	buf := make([]byte, bufferSize)
	n, err := conn.Read(buf)
	if err != nil {
		if errors.Is(err, net.ErrClosed) {
			log.Print("canceled")
			return nil
		}
		return err
	}

	log.Printf("read: (%d) %q", n, string(buf[:n]))

	return nil
}
