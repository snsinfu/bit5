package main

import (
	"os"
	"strings"
	"text/template"
)

func main() {
	// `.tmpl` seems to be the preferred extension for Go templates. Some
	// evidences:
	// - https://golang.org/pkg/text/template/#example_Template_glob
	// - https://github.com/sheerun/vim-polyglot/blob/v4.17.0/ftdetect/polyglot.vim#L2312
	filename := "sample.yaml.tmpl"

	// Template name must be set to filename. Otherwise, Execute() fails with
	// an error that reads `"" is an incomplete or empty template`. This is
	// not explicitly noted in the documentation but I guess it has to do with
	// ParseFiles setting filename as the name of the parsed template.
	// - https://golang.org/pkg/text/template/#ParseFiles
	tmpl := template.New(filename)

	// Functions must be set before calling ParseFiles(). Hence, we cannot use
	// template.ParseFiles() when defining custom functions.
	tmpl.Funcs(template.FuncMap{
		"split": strings.Split,
	})

	if _, err := tmpl.ParseFiles(filename); err != nil {
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
