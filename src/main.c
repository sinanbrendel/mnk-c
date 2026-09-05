#include "main.h"
#include "board.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

long get_input(void) {
  static char buffer[INPUT_SIZE];

  if (fgets(buffer, INPUT_SIZE, stdin) != NULL) {
    if (!strchr(buffer, '\n')) { // input was larger than buffer
      char c;
      while ((c = getchar()) != '\n' && c != EOF); // empty out stdin
    }
  }
  return strtol(buffer, NULL, 10);
}

void print_board(void) {
  const char player_1_char = 'X', player_2_char = 'O';

  for (int row = 0; row < N + 2; row++) { // + 2 for number rows
    for (int col = 0; col < M + 2; col++) { // + 2 for number cols
      if ((row <= 0 || row >= N + 1) && !(col <= 0 || col >= M + 1)) { // number rows
        printf(" %.2d ", col);
        continue;
      }
      if ((col <= 0 || col >= M + 1) && !(row <= 0 || row >= N + 1)) { // number cols
        printf(" %.2d ", row);
        continue;
      }

      char cell = ' ';
      if (get_cell(col, row) == PLAYER_1) cell = player_1_char;
      if (get_cell(col, row) == PLAYER_2) cell = player_2_char;
      printf(" %c ", cell); // cell
      if (col > 0 && col < M) printf("|"); // vertical separator
    }
    printf("\n");

    if (row < 1 || row > N - 1) continue; // skip horizontal separator
    for (int col = 0; col < M; col++) {
      if (col == 0) printf("    "); // offset to match numbered rows
      printf("---");
      if (col < M - 1) printf("+");
    }
    printf("\n");
  }
  printf("\n");
}

int main(void) {
  uint8_t x_pos, y_pos;
  board_init(M, N);
  game_init(K, P, Q);
  print_board();

  do {
    do {
      printf("Player: %d\n", current_player());
      printf("Enter x-position: ");
      x_pos = get_input();

      printf("Enter y-position: ");
      y_pos = get_input();
      printf("\n");
    } while (!set_cell(x_pos, y_pos, current_player()));
    print_board();
  } while (!check_win(x_pos, y_pos) && marks_left());
}
