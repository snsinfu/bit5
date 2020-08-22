package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"os/exec"

	"github.com/creack/pty"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {

	// Let's manipulate vi!
	c := exec.Command("vi")

	win := &pty.Winsize{
		Rows: 25,
		Cols: 80,
		X:    560,
		Y:    350,
	}

	pt, err := pty.StartWithSize(c, win)
	if err != nil {
		return err
	}
	defer pt.Close()

	go func() {
		// vi blocks until we read their output.
		io.Copy(ioutil.Discard, pt)
	}()

	pt.Write([]byte(":enew\n"))
	pt.Write([]byte("i"))
	pt.Write([]byte("The quick brown fox jumps over the lazy dog."))
	pt.Write([]byte{0x1b}) // ESC
	pt.Write([]byte(":w fox.txt\n"))
	pt.Write([]byte(":q\n"))
	pt.Write([]byte{0x04}) // end-of-transmission ^D

	if err := c.Wait(); err != nil {
		return err
	}

	fmt.Println("OK")

	return nil
}
