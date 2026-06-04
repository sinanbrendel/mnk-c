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

  printf("Enter x-position: ");
  x_pos = get_input();

  printf("Enter y-position: ");
  y_pos = get_input();

  Entry entry = {x_pos, y_pos, player};
  return entry;
}

void print_board(Entry entry) {
  long m = 15, n = 10;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < m; col++) {
      printf("   ");
      if (col < m - 1) printf("|");
    }
    printf("\n");

    if (row == n - 1) continue;
    for (int col = 0; col < m; col++) {
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
    printf("Player: %d\n", player2_turn);
    entry = get_next_entry(player2_turn);
    print_board(entry);
    player2_turn = !player2_turn;
  }
}

