#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

typedef enum {
    EMPTY,
    PLAYER_1,
    PLAYER_2
} mark_t;

void board_init(uint8_t m_rows, uint8_t n_cols);

bool set_cell(uint8_t x, uint8_t y, mark_t mark);

mark_t get_cell(uint8_t x, uint8_t y);

void reset(void);

#endif

