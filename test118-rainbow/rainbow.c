#include <stdio.h>
#include <unistd.h>


static const char up_and_erase[] = "\x1b[1A\x1b[2K";
static const char bg_bright_red[] = "\x1b[101m";
static const char bg_bright_green[] = "\x1b[102m";
static const char bg_bright_yellow[] = "\x1b[103m";
static const char bg_bright_blue[] = "\x1b[104m";
static const char bg_bright_magenta[] = "\x1b[105m";
static const char bg_bright_cyan[] = "\x1b[106m";
static const char reset[] = "\x1b[0m";

int main()
{
    enum { animation_length = 100 };
    enum { rainbow_width = 50 };

    const char *cycle[] = {
        bg_bright_red,
        bg_bright_yellow,
        bg_bright_green,
        bg_bright_cyan,
        bg_bright_blue,
        bg_bright_magenta,
    };
    enum { cycle_length = sizeof cycle / sizeof *cycle };

    printf("\n\n\n");

    for (int t = 0; t < animation_length; t++) {
        printf("%s", up_and_erase);
        printf("%s", up_and_erase);
        printf("%s", up_and_erase);

        // Beat
        printf("%s", cycle[(cycle_length - t % cycle_length) % cycle_length]);
        printf("%*s", rainbow_width, " ");
        printf("%s\n", reset);

        // Cycle (forward)
        for (int i = 0; i < rainbow_width; i++) {
            printf("%s ", cycle[(i + cycle_length - t % cycle_length) % cycle_length]);
        }
        printf("%s\n", reset);

        // Cycle (backward)
        for (int i = 0; i < rainbow_width; i++) {
            printf("%s ", cycle[(i + t) % cycle_length]);
        }
        printf("%s\n", reset);

        usleep(100000);
    }
}
