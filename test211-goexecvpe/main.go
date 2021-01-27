package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"syscall"
)

func main() {
	paths := strings.Split(os.Getenv("PATH"), ":")
	argv := []string{"echo", "Hello, world!"}
	envv := os.Environ()

	if err := execvpe(argv[0], paths, argv, envv); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
	}
}

// execvpe executes a file with given arguments and environment. It will
// search for an executable file in the given search paths if the file
// argument does not contain a path separator.
func execvpe(file string, paths, argv, envv []string) error {
	if dir, _ := filepath.Split(file); dir != "" {
		return syscall.Exec(file, argv, envv)
	}

	var err error = syscall.ENOENT

	for _, base := range paths {
		abspath := filepath.Join(base, file)
		err = syscall.Exec(abspath, argv, envv)

		if ec, ok := err.(syscall.Errno); ok {
			switch ec {
			case syscall.EACCES:
				continue
			case syscall.ENOENT:
				continue
			case syscall.ENOTDIR:
				continue
			}
		}
	}

	return err
}
