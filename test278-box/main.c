#include <stddef.h>
#include <stdio.h>

int main(void)
{
    // Box template.
    char const* const box[] = {
        "╭───────╮",
        "│ ╭───╮ │",
        "│ │   │ │",
        "│ ╰───╯ │",
        "╰───────╯",
    };
    size_t const rows = sizeof box / sizeof *box;

    // Rainbow xterm colors.
    int const palette[] = {
        197, 220, 82, 49, 33, 129
    };
    size_t const palette_size = sizeof palette / sizeof *palette;

    // Draw some copies of the box template with different colors.
    int const up = 4;
    int const right = 29;
    int const mod = 47;
    int const copies = 19;
    int offset = 10;

    for (size_t copy = 0; copy < copies; copy++) {
        if (copy > 0) {
            printf("\e[%dA", up);
            offset += right;
            offset %= mod;
        }

        printf("\e[38;5;%dm", palette[copy % palette_size]);

        for (size_t i = 0; i < rows; i++) {
            printf("\e[G\e[%dC%s", offset, box[i]);
            printf("\n");
        }
    }

    printf("\e[m");
}
