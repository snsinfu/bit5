import base64
import hashlib
import hmac
import os


def main():
    password = "correct horse battery staple"
    print(scram(password, "sha-1", os.urandom(16), 4096))
    print(scram(password, "sha-256", os.urandom(16), 4096))


def scram(password, hashtype, salt, rounds):
    """
    Hash password in RFC 5803 format.

    - https://tools.ietf.org/html/rfc3112
    - https://tools.ietf.org/html/rfc5802
    - https://tools.ietf.org/html/rfc5803
    """
    scheme, hash_id = determine_scram_scheme(hashtype)
    key = hashlib.pbkdf2_hmac(hash_id, password.encode("utf-8"), salt, rounds)
    server_key = hmac.digest(key, b"Server Key", hash_id)
    client_key = hmac.digest(key, b"Client Key", hash_id)
    stored_key = hashlib.new(hash_id, client_key).digest()
    auth_info = str(rounds) + ":" + base64str(salt)
    auth_value = base64str(stored_key) + ":" + base64str(server_key)
    return f"{scheme}${auth_info}${auth_value}"


def base64str(b):
    return base64.b64encode(b).decode("ascii")


def determine_scram_scheme(hashtype):
    if hashtype == "sha-1":
        return "SCRAM-SHA-1", "sha1"
    if hashtype == "sha-256":
        return "SCRAM-SHA-256", "sha256"
    raise Exception("unrecognized hash type")


main()
