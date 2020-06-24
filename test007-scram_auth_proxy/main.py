import os

import scram


def main():
    password = "correct horse battery staple"
    salt = os.urandom(16)
    rounds = 4096

    secret = scram.encrypt(password, salt, rounds)
    client = PasswordClient(password, salt, rounds)
    server = Server(secret)
    proxy = Proxy(server, secret)

    # Proxy can authenticate client against server without knowing plaintext
    # password. However, it must have the same hashed secret computed with
    # exactly the same password, salt and hash rounds.
    bad_proxy = Proxy(server, scram.encrypt(password, os.urandom(16), rounds))
    print("Authenticating correct client:")
    print(" server ->", server.auth(client))
    print(" proxy ->", proxy.auth(client))
    print(" bad proxy ->", bad_proxy.auth(client))
    print("")

    bad_client = PasswordClient("passw0rd", salt, rounds)
    print("Authenticating bad client:")
    print(" server ->", server.auth(bad_client))
    print(" proxy ->", proxy.auth(bad_client))


class KeyClient:
    """
    A client that only knows client key and does not know plaintext password.
    """
    def __init__(self, key):
        self._client_key = key

    def prove(self, msg):
        stored_key = scram.digest(self._client_key)
        signature = scram.sign(msg, stored_key)
        return scram.xor(self._client_key, signature)


class PasswordClient:
    """
    A client that knows plaintext password.
    """
    def __init__(self, password, salt, rounds):
        _, client_key = scram.derive_keys(password, salt, rounds)
        self._key_client = KeyClient(client_key)

    def prove(self, msg):
        return self._key_client.prove(msg)


class Server:
    """
    A server that authenticates user. It knows hashed secret.
    """
    def __init__(self, secret):
        self._secret = secret

    def auth(self, client):
        return self.verify_client_key(self.derive_client_key(client))

    def verify_client_key(self, client_key):
        return scram.digest(client_key) == self._secret.stored_key

    def derive_client_key(self, client):
        msg = os.urandom(16)
        proof = client.prove(msg)
        signature = scram.sign(msg, self._secret.stored_key)
        return scram.xor(proof, signature)


class Proxy(Server):
    """
    A server that authenticates user against another server. It knows hashed
    secret of the user but does not know plaintext password.
    """
    def __init__(self, origin, secret):
        self._origin = origin
        self._secret = secret

    def auth(self, client):
        client_key = self.derive_client_key(client)
        if not self.verify_client_key(client_key):
            return False
        return self._origin.auth(KeyClient(client_key))


main()
