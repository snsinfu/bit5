package main

import (
	"fmt"
	"os"
	"syscall"
)

func main() {
	path := os.Args[1]

	argv := []string{"echo", "Hello, world!"}
	env := os.Environ()
	err := syscall.Exec(path, argv, env)

	if ec, ok := err.(syscall.Errno); ok {
		switch ec {
		case syscall.EACCES:
			fmt.Fprintln(os.Stderr, "EACCES")
		case syscall.ENOENT:
			fmt.Fprintln(os.Stderr, "ENOENT")
		case syscall.ENOTDIR:
			fmt.Fprintln(os.Stderr, "ENOTDIR")
		default:
			fmt.Fprintf(os.Stderr, "error (%d): %s\n", ec, ec)
		}
	} else {
		fmt.Fprintln(os.Stderr, "error:", err)
	}

	os.Exit(111)
}
