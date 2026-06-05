#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_SIZE 20

typedef struct {
  long x;
  long y;
  bool player;
} Entry;

long get_input() {
  static char buffer[INPUT_SIZE];

  if (fgets(buffer, INPUT_SIZE, stdin) != NULL) {
    if (!strchr(buffer, '\n')) {
      char c;
      while ((c = getchar()) != '\n' && c != EOF);
    }
  }
  return strtol(buffer, NULL, 10);
}

Entry get_next_entry(bool player) {
  static long x_pos, y_pos;

  printf("Player: %d\n", player);
  printf("Enter x-position: ");
  x_pos = get_input();

  printf("Enter y-position: ");
  y_pos = get_input();
  printf("\n");

  Entry entry = {x_pos, y_pos, player};
  return entry;
}

void print_board(Entry entry) {
  long m = 15, n = 10;

  for (int row = 0; row < n + 2; row++) { // + 2 for number rows
    for (int col = 0; col < m + 2; col++) { // + 2 for number cols
      if ((row == 0 || row == n + 1) && (col > 0 && col < m + 1)) { // number rows
        printf(" %.2d ", col);
        continue;
      }
      if ((col == 0 || col == m + 1) && (row > 0 && row < n + 1)) { // number cols
        printf(" %.2d ", row);
        continue;
      }
      printf("   "); // cell
      if (col > 0 && col < m) printf("|"); // vertical separator
    }
    printf("\n");

    if (row < 1 || row > n - 1) continue; // skip horizontal separator
    for (int col = 0; col < m; col++) {
      if (col == 0) printf("    "); // offset to match numbered rows
      printf("---");
      if (col < m - 1) printf("+");
    }
    printf("\n");
  }
  printf("\n");
}

int main(void) {
  Entry entry;
  bool player2_turn = false;
  while (true) {
    entry = get_next_entry(player2_turn);
    print_board(entry);
    player2_turn = !player2_turn;
  }
}

