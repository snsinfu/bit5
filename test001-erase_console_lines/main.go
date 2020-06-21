package main

import "fmt"

// https://en.wikipedia.org/wiki/ANSI_escape_code
// https://en.wikipedia.org/wiki/ANSI.SYS

func main() {
	fmt.Println("1 The")
	fmt.Println("2 quick") // erased
	fmt.Println("3 brown") // erased
	fmt.Println("4 fox")   // erased

	erase(3)

	fmt.Println("2+ lazy")
	fmt.Println("3+ dog")
}

func erase(n int) {
	// "CSI n A CSI 2 K" only erases the last line and moves cursor n rows up.
	// So, to erase the entire n lines, we have to repeat single-line erase
	// command n times.
	for i := 0; i < n; i++ {
		fmt.Print("\x1b[1A\x1b[2K")
	}
}
