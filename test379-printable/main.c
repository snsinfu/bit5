#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


enum { BUFFER_SIZE = 4096 };
static const char hex_map[16] = "0123456789-=$!,.";

int main()
{
    for (;;) {
        uint8_t buffer[BUFFER_SIZE];

        size_t n_read = fread(buffer, 1, sizeof buffer, stdin);
        if (n_read == 0) {
            if (feof(stdin)) {
                break;
            }
            perror("fread");
            exit(1);
        }

        for (size_t i = 0; i < n_read; i++) {
            uint8_t ch = buffer[i];

            if (ch >= 64 && ch < 127) {
                putchar(ch);
            } else {
                putchar(hex_map[ch & 15]);
                putchar(hex_map[(ch >> 4) & 15]);
            }
        }
    }
}
