package main

import (
	"fmt"
	"os"

	"gopkg.in/yaml.v3"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	var config Config

	const sampleConfig1 = `
image: example/web
build:
  context: ./web
  args:
    GO_VERSION: "1.15"
`
	if err := yaml.Unmarshal([]byte(sampleConfig1), &config); err != nil {
		return err
	}
	fmt.Println(config)

	const sampleConfig2 = `
image: example/web
build: ./web
`
	if err := yaml.Unmarshal([]byte(sampleConfig2), &config); err != nil {
		return err
	}
	fmt.Println(config)

	return nil
}

// Config describes a container.
type Config struct {
	Image string
	Build BuildConfig
}

// Build describes how to build a container image.
type BuildConfig struct {
	Context string
	Args    map[string]string
}

func (bc *BuildConfig) UnmarshalYAML(node *yaml.Node) error {
	value := resolveAlias(node)

	// Short cut: Scalar string is interpreted as Context field.
	if value.Kind == yaml.ScalarNode {
		var str string
		if err := value.Decode(&str); err != nil {
			return err
		}
		*bc = BuildConfig{Context: str}
		return nil
	}

	if value.Kind == yaml.MappingNode {
		var data struct {
			Context string
			Args    map[string]string
		}
		if err := value.Decode(&data); err != nil {
			return err
		}
		*bc = BuildConfig{Context: data.Context, Args: data.Args}
		return nil
	}

	return fmt.Errorf("unmarshal error:\n"+
		"line %d: expected string or mapping", node.Line)
}

// resolveAlias resolves alias node to the non-alias node pointed to.
func resolveAlias(node *yaml.Node) *yaml.Node {
	for node.Kind == yaml.AliasNode {
		node = node.Alias
	}
	return node
}
