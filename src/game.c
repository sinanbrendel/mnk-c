#include "game.h"
#include <stdint.h>

static uint8_t k, p, marks_left_count;
static mark_t active_player = PLAYER_1;

void game_init(uint8_t win_condition_k, uint8_t marks_per_turn_p, uint8_t marks_first_turn_q) {
    k = win_condition_k;
    p = marks_per_turn_p;
    marks_left_count = marks_first_turn_q;
}

mark_t current_player(void) {
    return active_player;
}

uint8_t marks_left(void) {
    if (marks_left_count == 0) {
        active_player = (active_player % 2) + 1;
        marks_left_count = p;
    }
    return marks_left_count;
}

bool check_win(uint8_t x, uint8_t y) {
    marks_left_count--;
    // TODO: Implement actual win-checking
    if (x || y) return false; // remove useless usage of x and y to avoid compilation errors
    return false;
}
