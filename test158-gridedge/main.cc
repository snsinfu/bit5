// Thinking of a data structure for representing a grid game board where cells
// and edges both have states.
//
// +--+--+--+
// |  |  |  |
// +--+--+--+
// |  |  |  |
// +--+--+--+
// |  |  |  |
// +--+--+--+
//
// If the grid consists of n-by-n cells, the cells produce (n+1)-by-(n+1)
// edges. If we represent the cells and edges by two-dimensional arrays,
// then the cell at (i,j) is surrounded by the edges at (i,j), (i+1,j),
// (i,j+1) and (i+1,j+1).
//
// Probably the simplest and the dumbest deta structure. Let's see how it
// works.

#include <cassert>
#include <cstdlib>
#include <iostream>


static constexpr int board_size = 5;
static constexpr int mid_coord = board_size / 2;


enum class player_side : signed char
{
    none,
    red,
    black,
};

struct game_board
{
    player_side cells[board_size][board_size] = {};
    player_side edges[board_size + 1][board_size + 1] = {};

    int red_piece[2] = {};
    int black_piece[2] = {};
};

void put_piece(game_board& board, int x, int y, player_side side)
{
    int cur_x, cur_y;

    // This and similar switch statement in `move_piece()` smell. Needs a
    // better abstraction.
    switch (side) {
    case player_side::red:
        cur_x = board.red_piece[0];
        cur_y = board.red_piece[1];
        board.red_piece[0] = x;
        board.red_piece[1] = y;
        break;

    case player_side::black:
        cur_x = board.black_piece[0];
        cur_y = board.black_piece[1];
        board.black_piece[0] = x;
        board.black_piece[1] = y;
        break;

    case player_side::none:
        assert(0);
    }

    // This also smells. `put_piece` function handles moving logic?
    board.cells[cur_x][cur_y] = player_side::none;
    board.cells[x][y] = side;
}

void put_wall(game_board& board, int x, int y, player_side side)
{
    board.edges[x][y] = side;
}

bool move_piece(game_board& board, int dx, int dy, player_side side)
{
    int cur_x, cur_y;

    switch (side) {
    case player_side::red:
        cur_x = board.red_piece[0];
        cur_y = board.red_piece[1];
        break;

    case player_side::black:
        cur_x = board.black_piece[0];
        cur_y = board.black_piece[1];
        break;

    case player_side::none:
        assert(0);
    }

    assert(std::abs(dx) <= 1);
    assert(std::abs(dy) <= 1);

    // Player cannot move piece over a wall.
    if (dx != 0) {
        // Indices of the cell (x) and the surrounding edges look like this:
        //
        //       x  x+1
        //   x-1 | x | x+1
        //
        // So, if the piece is going right (dx == 1), we check the edge at
        // x+1. If the piece is going left (dx == -1), we check the edge at x.
        // So, we can use x + (dx+1)/2.
        auto const edge = board.edges[cur_x + (dx + 1) / 2][cur_y];
        if (edge != player_side::none) {
            return false;
        }
    }

    if (dy != 0) {
        // Same reasoning as above.
        auto const edge = board.edges[cur_x][cur_y + (dy + 1) / 2];
        if (edge != player_side::none) {
            return false;
        }
    }

    // OK. Move the piece.
    put_piece(board, cur_x + dx, cur_y + dy, side);

    return true;
}


int main()
{
    game_board board;

    put_piece(board, mid_coord, 0, player_side::red);
    put_piece(board, mid_coord, board_size - 1, player_side::black);

    // Manually calculating edge index may easily cause bugs. Maybe we let
    // the board compute edge from given cell and direction.
    put_wall(board, mid_coord, 1, player_side::black);

    bool ok1 = move_piece(board, 0, +1, player_side::red);
    bool ok2 = move_piece(board, 0, -1, player_side::black);
    assert(!ok1);
    assert(ok2);
}
