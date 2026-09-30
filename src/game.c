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

bool check_win(uint8_t new_mark_x, uint8_t new_mark_y) {
    marks_left_count--;
    int max = 1;
    for (int i = 0; i <= 1; i++) {
      for (int j = (i > 0) ? -1 : 0; j <= 1; j++) {
        if (i == 0 && j == 0) continue;
        int x = new_mark_x, y = new_mark_y, marks_in_a_row = 0;
        while (get_cell(x, y) == active_player && marks_in_a_row <= k) {
          marks_in_a_row++;
          x += i;
          y += j;
        }
        if (marks_in_a_row == k) return true;
        // reset x and y to initial position to keep checking in opposite direction
        // also reduce marks_in_a_row as the first loop iteration is always hit
        x = new_mark_x;
        y = new_mark_y;
        marks_in_a_row--;
        while (get_cell(x, y) == active_player && marks_in_a_row <= k) {
          marks_in_a_row++;
          x -= i;
          y -= j;
        }
        max = (marks_in_a_row > max) ? marks_in_a_row : max;
      }
    }
    return max >= k;
}
