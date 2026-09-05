#include "board.h"

#include <stdlib.h>
#include <string.h>

static mark_t* cells;
static uint8_t m, n;
static size_t board_size;

void board_init(uint8_t m_cols, uint8_t n_rows) {
    m = m_cols;
    n = n_rows;
    board_size = m * n * sizeof(mark_t);
    cells = malloc(board_size);
    if (cells == NULL) abort();
}

static bool cell_valid(uint8_t x, uint8_t y) {
    return x < m && y < n;
}

mark_t get_cell(uint8_t x, uint8_t y) {
    if (!cell_valid(x, y)) return EMPTY;
    return cells[y * m + x];
}

static bool cell_empty(uint8_t x, uint8_t y) {
    return cell_valid(x, y) && get_cell(x, y) == EMPTY;
}

bool set_cell(uint8_t x, uint8_t y, mark_t mark) {
    if (!cell_empty(x, y)) return false;
    cells[y * m + x] = mark;
    return true;
}

void reset(void) {
    memset(cells, 0, board_size);
}
