package main

import "fmt"

type Config struct {
	WindowSize struct{ X, Y int }
}

func main() {

	// This fails to compile
	// https://stackoverflow.com/questions/17912893/missing-type-in-composite-literal
	/*
	c := Config{
		WindowSize: {100, 200},
	}
	*/

	// OK
	c := Config{
		WindowSize: struct{ X, Y int }{100, 200},
	}

	// OK
	c.WindowSize.X = 100
	c.WindowSize.Y = 200

	fmt.Println(c)
}
