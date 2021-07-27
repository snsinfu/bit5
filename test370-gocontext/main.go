package main

// Signal-cancelable TCP echo server.

import (
	"context"
	"errors"
	"log"
	"net"
	"os"
	"os/signal"
	"time"
)

const (
	listenAddr    = "127.0.0.1:9500"
	bufferSize    = 1024
	connCloseWait = 3 * time.Second
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
	}
}

func run(ctx context.Context) error {
	addr, err := net.ResolveTCPAddr("tcp", listenAddr)
	if err != nil {
		return err
	}

	ln, err := net.ListenTCP("tcp", addr)
	if err != nil {
		return err
	}
	defer ln.Close()

	log.Printf("listening on %s", addr)

	go func() {
		<-ctx.Done()
		ln.Close()
	}()

	for {
		conn, err := ln.AcceptTCP()
		if err != nil {
			if errors.Is(err, net.ErrClosed) {
				break
			}
			return err
		}

		log.Printf("accepted connection from %s", conn.RemoteAddr())

		go func() {
			if err := echo(conn, ctx); err != nil {
				log.Printf("echo error: %s", err)
			}
		}()
	}

	log.Print("server terminated")

	// Need to wait for echo handlers to gracefully close connections. I could
	// (should?) use a synchronization primitive, but then I have to keep track
	// of all incoming connections. Also, there can always be non-responding
	// bad connections. So, maybe sleeping is a good enough compromise here...
	time.Sleep(connCloseWait)

	return nil
}

func echo(conn *net.TCPConn, ctx context.Context) error {
	defer conn.Close()

	go func() {
		<-ctx.Done()
		conn.Close()
	}()

	buf := make([]byte, bufferSize)

	for {
		n, err := conn.Read(buf)
		if err != nil {
			if errors.Is(err, net.ErrClosed) {
				break
			}
			return err
		}

		m, err := conn.Write(buf[:n])
		if err != nil {
			if errors.Is(err, net.ErrClosed) {
				break
			}
			return err
		}

		if m < n {
			log.Printf("short write! %d < %d", m, n)
		}
	}

	log.Printf("echo stopped (remote: %s)", conn.RemoteAddr())

	return nil
}
