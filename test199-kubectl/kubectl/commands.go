// Package kubectl wraps kubectl cli commands as Go functions.
package kubectl

import (
	"github.com/snsinfu/bit5/test199-kubectl/command"
)

type Apply struct {
	Manifests []string
	Kustomize string
	Namespace string
}

func (opts Apply) Run() error {
	cmd := command.Line{"kubectl", "apply"}

	for _, manifest := range opts.Manifests {
		cmd = append(cmd, "-f", manifest)
	}
	if opts.Kustomize != "" {
		cmd = append(cmd, "-k", opts.Kustomize)
	}
	if opts.Namespace != "" {
		cmd = append(cmd, "-n", opts.Namespace)
	}

	return cmd.Run()
}
