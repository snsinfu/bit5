package main

import (
	"crypto/ed25519"
	"encoding/base64"
	"fmt"
	"os"
)

// Reference keypair generated using the wg(8) command.
const (
	privKeyText = "WI2STMjqNdkI6cXiz8eY5aTSnA7wQvRj/47ER6FM90M="
	pubKeyText = "jUffZCebrylrk4qCE0wYWtrn7nECut9IHOaBT64xoCY="
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	refPrivSeed, err := base64.StdEncoding.DecodeString(privKeyText)
	if err != nil {
		return err
	}

	refPubKey, err := base64.StdEncoding.DecodeString(pubKeyText)
	if err != nil {
		return err
	}

	privKey := ed25519.NewKeyFromSeed(refPrivSeed)
	pubKey := privKey.Public()

	// Mismatch. Looks like wg keypair is not Ed25519. Curve25519?
	fmt.Println(refPubKey)
	fmt.Println(pubKey)

	return nil
}
