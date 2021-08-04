package main

import (
	"encoding/base64"
	"fmt"
	"mime/multipart"
	"net/textproto"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	mw := multipart.NewWriter(os.Stdout)
	defer mw.Close()

	// Looks like I have to compose all mime headers by myself.
	h := textproto.MIMEHeader{}
	h.Add("content-length", "12")
	h.Add("content-transfer-encoding", "base64")
	h.Add("content-type", "application/octet-stream")

	// Some extra header.
	h.Add("x-uuid", "d2351513-03d4-410d-b4a5-355c3eda7567")

	w, err := mw.CreatePart(h)
	if err != nil {
		return err
	}
	bw := base64.NewEncoder(base64.StdEncoding, w)
	bw.Write([]byte{1, 2, 3, 10, 20, 30, 100, 200})
	bw.Close()

	return nil
}
