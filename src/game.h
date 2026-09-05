#ifndef GAME_H
#define GAME_H

#include "board.h"

#include <stdint.h>

void game_init(uint8_t win_condition_k, uint8_t marks_per_turn_p, uint8_t marks_first_turn_q);

mark_t current_player(void);

uint8_t marks_left(void);

bool check_win(uint8_t x, uint8_t y);

#endif

