package command

import (
	"os"
	"os/exec"
	"strings"
)

type Line []string

func (cl Line) Run() error {
	cmd := exec.Command(cl[0], cl[1:]...)
	cmd.Stdout = os.Stdout
	cmd.Stdin = os.Stdin
	return cmd.Run()
}

func (cl Line) String() string {
	return strings.Join(cl, " ")
}
