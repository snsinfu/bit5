#include <stdio.h>

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
        .sin_port = htons(0),
        .sin_addr = (struct in_addr) {
            .s_addr = inet_addr("224.0.0.251"), // mDNS
        },
    };

    // DNS?
    char buf[] = {0};
    size_t len = sizeof buf;

    ssize_t nb_sent = sendto(
        sock, buf, len, 0, (const struct sockaddr *) &addr, sizeof addr
    );
    if (nb_sent == -1) {
        perror("sendto");
        goto done;
    }

    exit_code = 0;
done:
    close(sock);

    return exit_code;
}
