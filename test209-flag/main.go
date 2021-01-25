package main

import (
	"flag"
	"fmt"
)


type filesFlag []string

func (f *filesFlag) Set(val string) error {
	*f = append(*f, val)
	return nil
}

func (f *filesFlag) String() string {
	return ""
}

var files filesFlag

func init() {
	flag.Var(&files, "file", "File to process")
	flag.Var(&files, "f", "File to process")
}

func main() {
	flag.Parse()
	fmt.Println(files)
}
