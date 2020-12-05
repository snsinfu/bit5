// Using libssh.
//
// Note: There are libssh and libssh2. The former is *not* obsolete. These
// are different projects.
//
// https://stackoverflow.com/q/55669220
// https://www.libssh2.org/libssh2-vs-libssh.html

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libssh/libssh.h>


static int check_host_key(ssh_session session);


int
main(void)
{
    int exit_code = 1;
    ssh_session session = NULL;
    int err;

    session = ssh_new();
    if (session == NULL) {
        goto exit;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "localhost");

    err = ssh_connect(session);
    if (err) {
        fprintf(stderr, "error (ssh_connect): %s", ssh_get_error(session));
        goto exit;
    }

    err = check_host_key(session);
    if (err) {
        goto exit;
    }

    exit_code = 0;

exit:
    ssh_disconnect(session);
    ssh_free(session);
    return exit_code;
}


static int
check_host_key(ssh_session session)
{
    int result = -1;
    int err = -1;
    ssh_key host_key = NULL;
    unsigned char *hash = NULL;

    err = ssh_get_server_publickey(session, &host_key);
    if (err) {
        fprintf(stderr, "error ssh_get_server_publickey(%d)\n", err);
        goto exit;
    }

    size_t hash_size;
    err = ssh_get_publickey_hash(host_key, SSH_PUBLICKEY_HASH_SHA256, &hash, &hash_size);
    if (err) {
        fprintf(stderr, "error ssh_get_publickey_hash(%d)\n", err);
        goto exit;
    }

    printf("SHA256 of host key:\n  ");
    for (size_t i = 0; i < hash_size; i++) {
        if (i > 0) {
            printf(":");
        }
        printf("%02x", hash[i]);
    }
    printf("\n");

    result = 0;

exit:
    ssh_key_free(host_key);
    ssh_clean_pubkey_hash(&hash);
    return result;
}
