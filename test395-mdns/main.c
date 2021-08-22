// https://datatracker.ietf.org/doc/html/rfc6762
// https://github.com/mjansson/mdns

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


static uint8_t *encode_u16(uint8_t *buf, uint16_t value)
{
    buf[0] = (uint8_t) (value >> 8);
    buf[1] = (uint8_t) (value & 0xff);
    return buf + 2;
}


static uint8_t *encode_str(uint8_t *buf, const char *str)
{
    size_t len = strlen(str);
    *buf++ = (uint8_t) len;
    memcpy(buf, str, len);
    return buf + len;
}


static uint8_t slice_octet(uint32_t value, unsigned bit)
{
    return (uint8_t) (value >> bit);
}


static void format_addr(char *buf, const struct sockaddr_in *addr)
{
    const uint32_t ip = htonl(addr->sin_addr.s_addr);
    sprintf(
        buf,
        "%u.%u.%u.%u:%u",
        slice_octet(ip, 24),
        slice_octet(ip, 16),
        slice_octet(ip, 8),
        slice_octet(ip, 0),
        addr->sin_port
    );
}


int main()
{
    int exit_code = 1;
    int sock = -1;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        goto done;
    }

    const char *const mdns_broadcast = "224.0.0.251";
    const uint16_t mdns_port = 5353;

    struct sockaddr_in addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_port = htons(mdns_port),
        .sin_addr = (struct in_addr) {.s_addr = inet_addr(mdns_broadcast)},
    };

    // Send mDNS query. Let's find a chromecast.
    const char *name[] = {
        "_googlecast",
        "_tcp",
        "local",
    };
    const uint16_t query_id = 1111;

    {
        uint8_t buf[1500];
        uint8_t *end = buf;

        end = encode_u16(end, query_id);
        end = encode_u16(end, 1 << 8);
        end = encode_u16(end, 1);
        end = encode_u16(end, 0);
        end = encode_u16(end, 0);
        end = encode_u16(end, 0);

        size_t name_parts = sizeof name / sizeof *name;
        for (size_t i = 0; i < name_parts; i++) {
            end = encode_str(end, name[i]);
        }
        *end++ = 0;

        end = encode_u16(end, 12); // PTR
        end = encode_u16(end, 0x8001); // rrclass

        const size_t len = (size_t) (end - buf);

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
        char buf[1500];
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof addr;

        ssize_t nb_recv = recvfrom(
            sock, buf, sizeof buf, 0, (struct sockaddr *) &addr, &addr_len
        );
        if (nb_recv == -1) {
            perror("recvfrom");
            goto done;
        }

        char addr_str[32];
        format_addr(addr_str, &addr);
        printf("response from %s\n", addr_str);
    }

    exit_code = 0;
done:
    close(sock);

    return exit_code;
}
