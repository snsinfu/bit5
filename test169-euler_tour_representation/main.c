#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


struct node
{
    size_t  degree;
    size_t *adjacent_nodes;
};

static void euler_tour(const struct node *nodes, size_t cur, size_t parent);


int
main()
{
    // Euler tour: A tree is given. Starting from an arbitrary node, pass each
    // edge exactly two times and return to the initial node.
    //
    // 0
    //  \
    //   1
    //  / \
    // 2   3
    //    /|\
    //   4 5 6
    //      \
    //       7
    //
    // An Euler tour starting from 0 for this graph is:
    //
    //   0 1 2 1 3 4 3 5 7 5 3 6 3 1 0 .
    //
    // Note: Each consecutive pair in the Euler tour makes up an edge with
    // unique direction. I.e., if {i,j} is an edge in the tree, Euler tour
    // produces a path containing (i,j) and (j,i).
    //
    const size_t edges[][2] = {
        {0, 1},
        {1, 2},
        {1, 3},
        {3, 4},
        {3, 5},
        {3, 6},
        {5, 7},
    };
    const size_t edge_count = sizeof edges / sizeof *edges;
    const size_t node_count = 8;

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

    // Euler tour
    euler_tour(nodes, 0, 0);

    exit_code = 0;

out:
    free(nodes);
    free(conn_buffer);
    return exit_code;
}


void
euler_tour(const struct node *nodes, size_t cur, size_t parent)
{
    // Simple DFS to walk through the graph.

    const struct node *node = &nodes[cur];

    printf("%zu\n", cur);

    for (size_t i = 0; i < node->degree; i++) {
        size_t adj = node->adjacent_nodes[i];
        if (adj == parent) {
            continue;
        }

        euler_tour(nodes, adj, cur);

        printf("%zu\n", cur);
    }
}
