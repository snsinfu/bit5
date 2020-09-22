package main

import (
	"fmt"
	"os"
	"reflect"

	"crypto/sha1"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	// https://tools.ietf.org/html/rfc2202

	msg := []byte("Hi There")
	key := []byte{
		0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
		0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
		0x0b, 0x0b, 0x0b, 0x0b,
	}
	expect := []byte{
		0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64,
		0xe2, 0x8b, 0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e,
		0xf1, 0x46, 0xbe, 0x00,
	}

	mac := hmacSHA1(msg, key)
	fmt.Printf("% 02x\n", mac[:])

	if !reflect.DeepEqual(mac[:], expect) {
		panic("Incorrect HMAC!")
	}

	return nil
}

func hmacSHA1(msg, key []byte) [20]byte {
	const padSize = 64

	if len(key) > padSize {
		hashKey := sha1.Sum(key)
		key = hashKey[:]
	}

	ipad := make([]byte, padSize)
	opad := make([]byte, padSize)

	copy(ipad, key)
	copy(opad, key)

	for i := 0; i < padSize; i++ {
		ipad[i] ^= 0x36
		opad[i] ^= 0x5c
	}

	inner := sha1.New()
	inner.Write(ipad)
	inner.Write(msg)
	ihash := inner.Sum(nil)

	outer := sha1.New()
	outer.Write(opad)
	outer.Write(ihash)
	ohash := outer.Sum(nil)

	var mac [20]byte
	copy(mac[:], ohash)
	return mac
}
