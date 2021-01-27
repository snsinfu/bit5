# Golang syscall.Exec

Build:

```console
$ go build -o main .
```

Test successful case and various error modes:

```console
$ ./main /bin/echo
Hello, world!
$ ./main /etc/hosts
EACCES
$ ./main /bin/eeeechoooooo
ENOENT
$ ./main /bin/echo/echo
ENOTDIR
$ ./main /bin/$(echo {1..1000} | tr -d ' ')
error (63): file name too long
```
