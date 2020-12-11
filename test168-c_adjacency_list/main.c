#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


struct node
{
    size_t  degree;
    size_t *adjacent_nodes;
};


int main()
{
    // 0
    //  \
    //   1
    //    \
    //     2
    //    / \
    //   3   4
    const size_t edges[][2] = {
        {0, 1},
        {1, 2},
        {2, 3},
        {2, 4},
    };
    const size_t edge_count = sizeof edges / sizeof *edges;
    const size_t node_count = 5;

    int exit_code = 1;
    struct node *nodes = NULL;
    size_t *conn_buffer = NULL;

    nodes = malloc(node_count * sizeof *nodes);
    if (!nodes) {
        goto out;
    }

    conn_buffer = malloc(node_count * 2 * sizeof *conn_buffer);
    if (!conn_buffer) {
        goto out;
    }

    // Convert edge list to adjacency lists
    {
        size_t *conn = conn_buffer;

        for (size_t i = 0; i < node_count; i++) {
            size_t degree = 0;

            for (size_t j = 0; j < edge_count; j++) {
                const size_t *edge = edges[j];

                if (edge[0] == i) {
                    conn[degree++] = edge[1];
                }
                if (edge[1] == i) {
                    conn[degree++] = edge[0];
                }
            }

            nodes[i].degree = degree;
            nodes[i].adjacent_nodes = conn;
            conn += degree;
        }
    }

    // Print adjacency list of each node
    for (size_t i = 0; i < node_count; i++) {
        printf("%zu:", i);

        for (size_t j = 0; j < nodes[i].degree; j++) {
            printf(" %zu", nodes[i].adjacent_nodes[j]);
        }
        printf("\n");
    }

    exit_code = 0;

out:
    free(nodes);
    free(conn_buffer);
    return exit_code;
}
