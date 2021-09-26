#include <stdio.h>

// https://brhfl.com/2019/04/xterm-line-drawing/
// https://comp.databases.pick.narkive.com/ZAslgfwU/ot-vt100-line-box-drawing
// https://stackoverflow.com/a/13421360

int main() {
    // Graphics mode on: "(0" off: "(B"
    printf("\x1b(0");
    printf("lqqqqqqqqqqqqqqk");
    printf("\x1b(B");
    printf("\n");

    // SGR alternative font (this didn't work on my machine)
    printf("\x1b[12m");
    printf("mqqqqqqqqqqqqqqj");
    printf("\x1b[10m");
    printf("\n");
}
