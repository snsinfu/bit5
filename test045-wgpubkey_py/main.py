import base64

from cryptography.hazmat.primitives.asymmetric.x25519 import X25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat


# Reference keypair generated using wg(8) genkey/pubkey.
REF_PRIVKEY_TEXT = "WI2STMjqNdkI6cXiz8eY5aTSnA7wQvRj/47ER6FM90M="
REF_PUBKEY_TEXT = "jUffZCebrylrk4qCE0wYWtrn7nECut9IHOaBT64xoCY="

reference_privkey = base64.b64decode(REF_PRIVKEY_TEXT.encode("ascii"))
reference_pubkey = base64.b64decode(REF_PUBKEY_TEXT.encode("ascii"))

x = X25519PrivateKey.from_private_bytes(reference_privkey)
p = x.public_key()

derived_pubkey = p.public_bytes(Encoding.Raw, PublicFormat.Raw)

if derived_pubkey == reference_pubkey:
    print("OK")
else:
    print("Mismatch")
