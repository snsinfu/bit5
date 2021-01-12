package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"strings"

	"gopkg.in/yaml.v3"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}

func run() error {
	data, err := ioutil.ReadFile("Build.yaml")
	if err != nil {
		return err
	}

	config := buildConfig{}
	if err := yaml.Unmarshal(data, &config); err != nil {
		return err
	}

	for _, image := range config.Images {
		cmd := []string{"docker", "build", "-t", image.Name, image.Build}
		if err := runCommand(cmd); err != nil {
			return err
		}
	}

	namespace := config.Kubernetes.Namespace

	for _, manifest := range config.Kubernetes.Apply {
		cmd := []string{"kubectl", "apply", "-f", manifest}
		if namespace != "" {
			cmd = append(cmd, "-n", namespace)
		}
		if err := runCommand(cmd); err != nil {
			return err
		}
	}

	for _, target := range config.Kubernetes.Restart {
		cmd := []string{"rollout", "restart", target}
		if namespace != "" {
			cmd = append(cmd, "-n", namespace)
		}
		if err := runCommand(cmd); err != nil {
			return err
		}
	}

	return nil
}

func runCommand(cmdline []string) error {
	cmd := exec.Command(cmdline[0], cmdline[1:]...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	fmt.Fprintf(os.Stderr, "%s\n", strings.Join(cmd.Args, " "))

	// Dry run.
	// return cmd.Run()
	return nil
}

type buildConfig struct {
	Images []struct {
		Name  string
		Build string
	}

	Kubernetes struct {
		Namespace string
		Apply     anyString
		Restart   anyString
	}
}

type anyString []string

func (a *anyString) UnmarshalYAML(node *yaml.Node) error {
	value := resolveAlias(node)

	if value.Kind == yaml.SequenceNode {
		var seq []string
		if err := value.Decode(&seq); err != nil {
			return err
		}
		*a = seq
		return nil
	}

	if value.Kind == yaml.ScalarNode {
		var str string
		if err := value.Decode(&str); err != nil {
			return err
		}
		*a = anyString{str}
		return nil
	}

	return fmt.Errorf(
		"unmarshal error:\n"+
			"  line %d: expected string or sequence of strings",
		node.Line,
	)
}

func resolveAlias(node *yaml.Node) *yaml.Node {
	for node.Kind == yaml.AliasNode {
		node = node.Alias
	}
	return node
}
