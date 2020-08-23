package main

// https://tools.ietf.org/html/draft-miller-ssh-agent-04

import (
	"errors"
	"fmt"
	"net"
	"io"
	"os"

	"github.com/snsinfu/bit5/test073-sshagent/pack"
)

const (
	SSH_AGENTC_REQUEST_IDENTITIES = 11
	SSH_AGENT_IDENTITIES_ANSWER   = 12
	SSH_AGENTC_SIGN_REQUEST       = 13
	SSH_AGENT_SIGN_RESPONSE       = 14

	maxMessageSize = 65536
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

	// Response
	//
	// uint32  message length
	// byte    message number (SSH_AGENT_IDENTITIES_ANSWER)
	// uint32  number of keys
	// {
	//   uint32   key length
	//   byte[*]  binary pubkey
	//   uint32   comment length
	//   byte[*]  comment string
	// }

	buf, err := readMessage(conn)
	if err != nil {
		return err
	}

	recv := pack.NewReader(buf)

	msgCode, _ := recv.ReadByte()
	if msgCode != SSH_AGENT_IDENTITIES_ANSWER {
		return errors.New("unexpected message")
	}

	nkeys, _ := recv.Read32()
	for i := uint32(0); i < nkeys; i++ {
		keyLen, _ := recv.Read32()
		key := make([]byte, int(keyLen))
		recv.ReadExact(key)

		commentLen, _ := recv.Read32()
		comment := make([]byte, int(commentLen))
		recv.ReadExact(comment)

		fmt.Printf("[%d] %q %x\n", i, string(comment), key)
	}

	return nil
}

func readMessage(r io.Reader) ([]byte, error) {
	var sizeBuf [4]byte
	if err := readExact(r, sizeBuf[:4]); err != nil {
		return nil, err
	}

	size := uint32(sizeBuf[0]) << 24 | uint32(sizeBuf[1]) << 16 | uint32(sizeBuf[2]) << 8 | uint32(sizeBuf[3])
	if size > maxMessageSize {
		return nil, errors.New("message too large")
	}

	buf := make([]byte, int(size))
	if err := readExact(r, buf); err != nil {
		return nil, err
	}

	return buf, nil
}

func readExact(r io.Reader, buf []byte) error {
	n, err := r.Read(buf)
	if err != nil {
		return err
	}
	if n < len(buf) {
		return errors.New("short input")
	}
	return nil
}
