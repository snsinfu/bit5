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

	log.Fatal(http.ListenAndServe(":15343", nil))
}

func session(ws *websocket.Conn) {
	defer ws.Close()

	ticker := time.NewTicker(messageInterval)
	defer ticker.Stop()

	for t := range ticker.C {
		msg := fmt.Sprintf("Message at %s", t)

		if err := ws.WriteMessage(websocket.TextMessage, []byte(msg)); err != nil {
			log.Print("WriteMessage: ", err)
			break
		}

		log.Print("Sent message")
	}
}
