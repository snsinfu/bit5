package main

import (
	"github.com/snsinfu/bit5/test199-kubectl/docker"
	"github.com/snsinfu/bit5/test199-kubectl/kubectl"
)

func main() {
	build := docker.Build{
		Context: ".",
		Name:    "private-registry:5000/myapp/web",
		Args: map[string]string{
			"GO_VERSION": "1.14",
		},
	}
	build.Run()

	apply := kubectl.Apply{
		Manifests: []string{"./manifests"},
		Namespace: "myapp",
	}
	apply.Run()
}
