package main

import (
	"os"
	"strings"
	"text/template"
)

const source = `
{{- $a := index (splitOnce .text ":") 0 }}
{{- $b := index (splitOnce .text ":") 1 }}
a = {{ $a }}
b = {{ $b }}
`

func main() {
	tmpl := template.New("")

	tmpl.Funcs(template.FuncMap{
		// Template function can't return multiple values. Error message says
		// `can't install method/function "splitOnce" with 2 results`.
		/*
		"splitOnce": func(s, sep string) []string {
			l, r := splitOnce(s, sep)
			return []string{l, r}
		},
		*/
		// Returning array works but `splitN` would be better then?
		"splitOnce": func(s, sep string) []string {
			l, r := splitOnce(s, sep)
			return []string{l, r}
		},
	})

	if _, err := tmpl.Parse(source); err != nil {
		panic(err)
	}

	vars := map[string]interface{}{
		"text": "aaaa:bbbb:1234",
	}

	if err := tmpl.Execute(os.Stdout, vars); err != nil {
		panic(err)
	}
}

func splitOnce(s, sep string) (string, string) {
	pos := strings.Index(s, sep)
	if pos == -1 {
		return s, ""
	}
	return s[:pos], s[pos+1:]
}
