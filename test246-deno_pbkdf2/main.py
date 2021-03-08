import binascii
import hashlib

password = "password"
salt = "9a47f7cd8885a288514ec835fa8a8c84"
rounds = 10000
key = hashlib.pbkdf2_hmac(
    "sha256",
    password.encode("utf-8"),
    binascii.unhexlify(salt),
    rounds,
)

print(binascii.hexlify(key).decode("utf-8"))
