package main

import (
	"fmt"
	"os"

	"crypto/sha1"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	// https://tools.ietf.org/html/rfc6238
	key := []byte("12345678901234567890")
	counter := uint64(1234567890 / 30)
	digits := 8

	totp := fmt.Sprintf("%0.*d", digits, computeHOTP(counter, key))
	totp = totp[len(totp)-digits:]

	// "89005924"
	fmt.Println(totp)

	return nil
}

func computeHOTP(counter uint64, key []byte) uint32 {
	// Big endian
	msg := make([]byte, 8)
	for i := 0; i < 8; i++ {
		msg[i] = byte(counter >> (56 - i*8))
	}
	return truncate(hmacSHA1(msg, key))
}

func truncate(hs []byte) uint32 {
	offset := hs[len(hs)-1] & 0xf

	// Big endian
	p := uint32(0)
	for i := offset; i < offset+4; i++ {
		p <<= 8
		p |= uint32(hs[i])
	}

	return p & 0x7fffffff
}

func hmacSHA1(msg, key []byte) []byte {
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

	return ohash
}
