#include <stdio.h>
#include <string.h>


int main(void) {

    const char program[] =
        "ypnyismmpvqqurskjsumgkxnbljpvyvdcjnpusxltrbznastfbzkcspxbjxbrwmt";

    for (size_t i = 0; i < strlen(source); i++) {
        printf("%zu\t", i);

        // Generate a strchr sequence starting from the i-th char.
        const char *p = program;
        const char *q = program + i;

        do {
            // Transition rule uses strchr.
            char c = *q;
            q = p + 1;
            p = strchr(p, c);
            putchar(c);
        } while (p >= q);

        printf("\n");
    }

    return 0;
}
