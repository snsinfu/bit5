#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


int main()
{
    int exit_code = 1;
    int sock = -1;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        goto done;
    }

    struct sockaddr_in addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_port = htons(53),
        .sin_addr = (struct in_addr) {
            .s_addr = inet_addr("224.0.0.251"), // mDNS
        },
    };

    // Send DNS query.

    // FIXME: `dns-sd -B _googlecast._tcp local` works while this code does not.

    const char *query_components[] = {
        "_googlecast",
        "_tcp",
        "local",
    };
    {
        uint8_t buf[512];
        size_t len = 0;

        *(uint16_t *) (buf + len) = htons(1111);
        len += 2;
        *(uint16_t *) (buf + len) = htons(1 << 8);
        len += 2;
        *(uint16_t *) (buf + len) = htons(1);
        len += 2;
        *(uint16_t *) (buf + len) = htons(0);
        len += 2;
        *(uint16_t *) (buf + len) = htons(0);
        len += 2;
        *(uint16_t *) (buf + len) = htons(0);
        len += 2;

        size_t n_components = sizeof query_components / sizeof *query_components;
        for (size_t i = 0; i < n_components; i++) {
            const char *component = query_components[i];
            size_t component_len = strlen(component);
            buf[len++] = (uint8_t) component_len;
            memcpy(buf + len, component, component_len);
            len += component_len;
        }
        buf[len++] = 0;

        *(uint16_t *) (buf + len) = htons(1);
        len += 2;
        *(uint16_t *) (buf + len) = htons(1);
        len += 2;

        ssize_t nb_sent = sendto(
            sock, buf, len, 0, (const struct sockaddr *) &addr, sizeof addr
        );
        if (nb_sent == -1) {
            perror("sendto");
            goto done;
        }
    }

    // Receive response.
    {
        char buf[1024];
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof addr;

        ssize_t nb_recv = recvfrom(
            sock, buf, sizeof buf, 0, (struct sockaddr *) &addr, &addr_len
        );
        if (nb_recv == -1) {
            perror("recvfrom");
            goto done;
        }
    }

    exit_code = 0;
done:
    close(sock);

    return exit_code;
}
