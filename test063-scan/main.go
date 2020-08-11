package main

import (
	"fmt"
	"regexp"
	"strings"
)

const source = `
#version 330
#pragma optimize(off)
#pragma foo:bar 123
 # pragma foo:baz 456
  #  pragmaaaa
// #pragma foo:qux 789

void main() {
}
`

const spaceChars = " \t"

func main() {
	parsePragma(source)
}

func parsePragma(s string) error {
	var rePragmaFoo = regexp.MustCompile(`^\s*#\s*pragma\s+foo:(\w+)\s+(.*)`)

	for _, line := range strings.Split(s, "\n") {
		match := rePragmaFoo.FindStringSubmatch(line)
		if match != nil {
			fmt.Println(match[1:])
		}
	}

	return nil
}
