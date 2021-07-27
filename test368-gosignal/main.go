package main

import (
	"errors"
	"log"
	"net"
	"os"
	"os/signal"
)

func main() {
	p := NewProcess()

	sigCh := make(chan os.Signal)
	signal.Notify(sigCh, os.Interrupt)
	go func() {
		for range sigCh {
			p.Stop()
		}
	}()

	p.Start()
}

type Process struct {
	listener *net.TCPListener
	stop     chan bool
}

func NewProcess() *Process {
	return &Process{
		stop: make(chan bool),
	}
}

func (p *Process) Start() {
	go func() {
		<-p.stop
		// listener may be null in a tight time window...
		p.listener.Close()
	}()

	err := p.listen()

	// SIGINT closes the listener, resulting in an ErrClosed error.
	if errors.Is(err, net.ErrClosed) {
		log.Printf("stopped")
		return
	}

	if err != nil {
		log.Printf("error: %s", err)
	}
}

func (p *Process) Stop() {
	p.stop <- true
}

func (p *Process) listen() error {
	addr, err := net.ResolveTCPAddr("tcp", "127.0.0.1:9999")
	if err != nil {
		return err
	}

	ln, err := net.ListenTCP("tcp", addr)
	if err != nil {
		return err
	}
	defer ln.Close()

	log.Printf("listening on %s", addr)

	p.listener = ln

	for {
		conn, err := ln.AcceptTCP()
		if err != nil {
			return err
		}

		go func() {
			log.Printf("accepted connection from %s", conn.RemoteAddr())
			conn.Close()
		}()
	}
}
