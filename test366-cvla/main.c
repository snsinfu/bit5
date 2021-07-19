#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


void zero_matrix(size_t rows, size_t cols, float mat[rows][cols])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            mat[i][j] = 0;
        }
    }
}


// This does not work.
/*
struct matrix
{
    size_t rows;
    size_t cols;
    float(* elements)[cols];
};
*/

int main(void)
{
    size_t rows = 100;
    size_t cols = 10;

    float(*mat)[cols] = malloc(rows * sizeof *mat);
    printf("%p\n", (void *) mat);
    zero_matrix(rows, cols, mat);
    free(mat);
}
