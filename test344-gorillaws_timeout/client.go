package main

import (
	"log"
	"io"
	"os"

	"github.com/gorilla/websocket"
)

const (
	websocketURL = "ws://localhost:15343/"
)

func main() {
	if err := run(); err != nil {
		log.Print("Error: ", err)
		os.Exit(1)
	}
}

func run() error {
	ws, _, err := websocket.DefaultDialer.Dial(websocketURL, nil)
	if err != nil {
		return err
	}
	defer ws.Close()

	buf := make([]byte, 1024)

	for {
		_, r, err := ws.NextReader()
		if err != nil {
			return err
		}

		for {
			n, err := r.Read(buf)
			if err != nil {
				if err == io.EOF {
					break
				}
				return err
			}

			text := string(buf[:n])
			log.Print("Read: ", text)
		}
	}

	log.Print("Finished")

	return nil
}
