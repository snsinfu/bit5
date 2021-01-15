// Package docker wraps docker cli commands as Go functions.
package docker

import (
	"github.com/snsinfu/bit5/test199-kubectl/command"
)

type Build struct {
	Context string
	Name    string
	Args    map[string]string
}

func (opts Build) Run() error {
	cmd := command.Line{"docker", "build"}

	if opts.Name != "" {
		cmd = append(cmd, "-t", opts.Name)
	}
	for key, value := range opts.Args {
		cmd = append(cmd, "--build-arg", key+"="+value)
	}
	cmd = append(cmd, opts.Context)

	return cmd.Run()
}
