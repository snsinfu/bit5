package main

import "fmt"

func main() {
	fmt.Println("main started")

	// Function calls in a method chain in a defer statement are immediately
	// invoked except for the last one, which is invoked later.
	defer start(10).stop()

	fmt.Println("main ended")
}

type task struct {
	id int
}

func start(id int) *task {
	fmt.Printf("task %d started\n", id)
	return &task{id}
}

func (t *task) stop() {
	fmt.Printf("task %d stopped\n", t.id)
}
