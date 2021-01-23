package main

import (
	"os"
	"strings"
	"text/template"
)

const source = `
port: {{ .PORT }}
names:
  {{- range $name := split .NAMES "," }}
    - {{ $name }}
  {{- end }}
`

func main() {
	tmpl := template.New("")

	tmpl.Funcs(template.FuncMap{
		"split": strings.Split,
	})

	if _, err := tmpl.Parse(source); err != nil {
		panic(err)
	}

	vars := map[string]interface{}{
		"PORT":  8080,
		"NAMES": "alice,bob,charlie",
	}

	if err := tmpl.Execute(os.Stdout, vars); err != nil {
		panic(err)
	}
}
