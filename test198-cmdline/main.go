package main

import (
	"fmt"

	"github.com/snsinfu/bit5/test198-cmdline/command"
)

func main() {
	cmd := command.Line{"find", "."}
	cmd = append(cmd, "-name", "*.go")
	cmd = append(cmd, "-print")

	fmt.Println(">", cmd)

	if err := cmd.Run(); err != nil {
		fmt.Println("error:", err)
	}
}
