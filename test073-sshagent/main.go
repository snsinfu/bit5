package main

// https://tools.ietf.org/html/draft-miller-ssh-agent-04

import (
	"fmt"
	"net"
	"os"

	"github.com/snsinfu/bit5/test073-sshagent/pack"
)

const (
	SSH_AGENTC_REQUEST_IDENTITIES = 11
	SSH_AGENT_IDENTITIES_ANSWER   = 12
	SSH_AGENTC_SIGN_REQUEST       = 13
	SSH_AGENT_SIGN_RESPONSE       = 14
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	conn, err := net.Dial("unix", os.Getenv("SSH_AUTH_SOCK"))
	if err != nil {
		return err
	}
	defer conn.Close()

	// List public keys.

	send := pack.NewWriter()
	send.Write32(1)
	send.WriteByte(SSH_AGENTC_REQUEST_IDENTITIES)

	if _, err := conn.Write(send.Bytes()); err != nil {
		return err
	}

	buf := make([]byte, 4096)
	n, err := conn.Read(buf)
	if err != nil {
		return err
	}

	fmt.Println(buf[:n])

	return nil
}
