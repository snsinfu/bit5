package main

import (
	"encoding/base64"
	"fmt"
	"os"
	"reflect"

	"golang.org/x/crypto/curve25519"
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
	refPrivKey, err := base64.StdEncoding.DecodeString(privKeyText)
	if err != nil {
		return err
	}

	refPubKey, err := base64.StdEncoding.DecodeString(pubKeyText)
	if err != nil {
		return err
	}

	pubKey, err := curve25519.X25519(refPrivKey, curve25519.Basepoint)
	if err != nil {
		return err
	}

	// Match. So, a wireguard keypair is a pair of random scalar (privkey) and
	// the multiplication of the scalar and the Curve25519 base point (pubkey).
	fmt.Println(refPubKey)
	fmt.Println(pubKey)

	if reflect.DeepEqual(pubKey, refPubKey) {
		fmt.Println("OK")
	} else {
		fmt.Println("Mismatch")
	}

	return nil
}
