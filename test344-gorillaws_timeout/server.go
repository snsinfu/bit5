package main

import (
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

const (
	messageInterval = 5 * time.Second
	pingPongTimeout = 3 * time.Second
	patience = 5
)

func main() {
	upgrader := websocket.Upgrader{}

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		log.Print("Received request")
		ws, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			log.Print("Upgrade error: ", err)
		}
		log.Print("Start session")
		go session(ws)
	})

	log.Print("Listening")

	log.Fatal(http.ListenAndServe(":15343", nil))
}

func session(ws *websocket.Conn) {
	defer ws.Close()

	ticker := time.NewTicker(messageInterval)
	defer ticker.Stop()

	ping := time.NewTicker(pingPongTimeout)
	defer ping.Stop()

	defer log.Print("End session")

	ws.SetPongHandler(func(string) error {
		log.Print("Got pong")
		ws.SetReadDeadline(time.Now().Add(pingPongTimeout))
		return nil
	})

	noPong := make(chan bool)

	go func() {
		counter := 0
		for {
			if _, _, err := ws.NextReader(); err != nil {
				counter++
				log.Print("ReadMessage: ", err)
				if counter > patience {
					noPong <- true
					break
				}
				time.Sleep(pingPongTimeout) // ?
			} else {
				counter = 0
			}
		}
	}()

	for {
		select {
		case t := <-ticker.C:
			msg := fmt.Sprintf("Message at %s", t)

			if err := ws.WriteMessage(websocket.TextMessage, []byte(msg)); err != nil {
				log.Print("WriteMessage: ", err)
				return
			}

			log.Print("Sent message")

		case t := <-ping.C:
			ws.SetReadDeadline(time.Now().Add(pingPongTimeout))
			if err := ws.WriteControl(websocket.PingMessage, nil, t.Add(pingPongTimeout)); err != nil {
				log.Print("WriteControl: ", err)
				return
			}
			log.Print("Sent ping")

		case <-noPong:
			log.Print("No pong")
		}
	}
}
