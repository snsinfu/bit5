import base64
import hashlib
import hmac
import os

from dataclasses import dataclass


SCRAM_SCHEME = "SCRAM-SHA-256"
HASH_TYPE = "sha256"


@dataclass
class Secret:
    rounds: int
    salt: bytes
    stored_key: bytes
    server_key: bytes

    def __str__(self):
        auth_info = str(self.rounds) + ":" + base64str(self.salt)
        auth_value = base64str(self.stored_key) + ":" + base64str(self.server_key)
        return f"{SCRAM_SCHEME}${auth_info}${auth_value}"


def encrypt(password, salt, rounds):
    server_key, client_key = derive_keys(password, salt, rounds)
    stored_key = digest(client_key)
    return Secret(rounds, salt, stored_key, server_key)


def derive_keys(password, salt, rounds):
    key = hashlib.pbkdf2_hmac(HASH_TYPE, password.encode("utf-8"), salt, rounds)
    server_key = sign(b"Server Key", key)
    client_key = sign(b"Client Key", key)
    return server_key, client_key


def digest(data):
    return hashlib.new(HASH_TYPE, data).digest()


def sign(data, key):
    return hmac.digest(key, data, HASH_TYPE)


def xor(xs, ys):
    return bytes(x ^ y for x, y in zip(xs, ys))


def base64str(b):
    return base64.b64encode(b).decode("ascii")
