#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum
{
    n_rows = 9,
    n_cols = 17,
};


static int
clamp(int value, int min, int max)
{
    return
        value < min ? min :
        value > max ? max :
        value;
}


static void
drunken_bishop(
    const uint8_t *hash, size_t size, char board[n_rows][n_cols]
)
{
    int i = n_rows / 2;
    int j = n_cols / 2;

    // Shift initial position using the global sum of the hash.
    {
        int sum = 0;

        for (size_t i = 0; i < size; i++) {
            sum += (int) hash[i];
        }

        const int sum_hi = sum >> 4;
        const int sum_lo = sum;

        i += sum_hi % (n_rows / 2) - n_rows / 4;
        j += sum_lo % (n_cols / 2) - n_cols / 4;
    }

    // Drunken bishop.
    const int delta_i[4] = {-1, -1, +1, +1};
    const int delta_j[4] = {-1, +1, -1, +1};

    memset(board, 0, n_rows * n_cols);

    for (size_t octet = 0; octet < size; octet++) {
        uint8_t ops = hash[octet];

        for (size_t slice = 0; slice < 4; slice++) {
            i = clamp(i + delta_i[ops & 3], 0, n_rows - 1);
            j = clamp(j + delta_j[ops & 3], 0, n_cols - 1);
            ops >>= 2;
            board[i][j] += 1;
        }
    }

    // Map to ASCII chars.
    const char charset[] = " .o+=*B0X@%&#/^";
    const size_t charset_len = strlen(charset);

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            board[i][j] = charset[(size_t) board[i][j] % charset_len];
        }
    }
}


static
void print_board(const char board[n_rows][n_cols])
{
    putchar('+');
    for (size_t j = 0; j < n_cols; j++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');

    for (size_t i = 0; i < n_rows; i++) {
        putchar('|');
        for (size_t j = 0; j < n_cols; j++) {
            putchar(board[i][j]);
        }
        putchar('|');
        putchar('\n');
    }

    putchar('+');
    for (size_t j = 0; j < n_cols; j++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
}


int
main()
{
    enum { hash_size = 16 };
    uint8_t hash[hash_size];
    {
        FILE *rand = fopen("/dev/urandom", "r");
        fread(hash, 1, hash_size, rand);
        fclose(rand);
    }

    char board[n_rows][n_cols];

    drunken_bishop(hash, sizeof hash, board);
    print_board(board);

    hash[hash_size - 1] ^= 1;

    drunken_bishop(hash, sizeof hash, board);
    print_board(board);
}
