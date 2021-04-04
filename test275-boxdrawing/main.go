package main

import (
	"fmt"
)

// https://en.wikipedia.org/wiki/Box-drawing_character

func main() {
	rows := []string{
		"┌──────┬───┐",
		"│      │   │",
		"│      │   │",
		"├──────┼───┤",
		"│      │   │",
		"└──────┴───┘",
		"",
		" ╱╲╱╲╱╲╱╲╱╲╲",
		"╱          ╲╲",
		"╲╲ ╱╲ ╱╲    ╲╲",
		" ╲╳  ╳  ╳╲   ╲╲",
		"   ╲╱ ╲╱ ╲╲   ╳",
		"          ╲╲ ╱ ╲",
		"           ╲╱   ╲",
		"╭───────╮",
		"│ ╭───╮ │",
		"│ │   │ │",
		"│ ╰───╯ │",
		"╰───────╯",
	}
	for _, s := range rows {
		fmt.Printf("   %s\n", s)
	}
}
