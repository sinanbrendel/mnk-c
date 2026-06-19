#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define M 20
#define N 20
#define K 3
#define P 4
#define Q 2
#define INPUT_SIZE 20

uint_least8_t board[M][N] = {};

bool validate_coord(long x, long y) {
  return (((x >= 0 && x < M) && (y >= 0 && y < N)) && board[y][x] == 0);
}

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

bool get_next_entry(bool player) {
  static long x_pos, y_pos;

  do {
    printf("Player: %d\n", player);
    printf("Enter x-position: ");
    x_pos = get_input() - 1;

    printf("Enter y-position: ");
    y_pos = get_input() - 1;
    printf("\n");
  } while (!validate_coord(x_pos, y_pos));

  board[y_pos][x_pos] = player + 1;
  return true;
}

void print_board(void) {
  const char player_1 = 'X', player_2 = 'O';

  for (int row = 0; row < N + 2; row++) { // + 2 for number rows
    for (int col = 0; col < M + 2; col++) { // + 2 for number cols
      if ((row <= 0 || row >= N + 1) && !(col <= 0 || col >= M + 1)) { // number rows
        // handled case "is in left and right panels, and not in top and bottom panels"
        printf(" %.2d ", col);
        continue;
      }
      if ((col <= 0 || col >= M + 1) && !(row <= 0 || row >= N + 1)) { // number cols
        // handled case "is in top and bottom panels, and not in left and right panels"
        printf(" %.2d ", row);
        continue;
      }
      if ((col <= 0 || col >= M + 1) || (row <= 0 || row >= N + 1)) { // number cols
        // now handle case "is in any panels" (aka the corners, because the other cases are dealt with)
        printf(" # ", row);
        continue;
      }

      char cell = ' ';
      if (board[row - 1][col - 1] == 1) cell = player_1;
      if (board[row - 1][col - 1] == 2) cell = player_2;
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

void find_winner(int* winner, int* start_row, int* start_col, int* end_row, int* end_col) {
  //check all rows
  for (int row = 0; row < N; row++) {
    int current_length = 0;

    if (board[row][0] != 0) current_length++; //first occurence of player

    for (int col = 1; col < M; col++) {
      if (board[row][col] == 0) current_length = 0; //reset because no player
      else if (board[row][col-1] == board[row][col]) current_length++; //another occurence
      else current_length = 1; //first occurence of player

      if (current_length >= K) {
        *winner = board[row][col];
        *end_row = row+1;
        *end_col = col+1;
        *start_row = row+1;
        *start_col = (col+1) - current_length + 1;
        //printf("curr_length: %d, winner = %d, start_row = %d, start_col = %d, end_row = %d, end_col = %d\n", current_length, winner, *start_row, *start_col, *end_row, *end_col);
        return;
      }

    }
  }

  //check all columns
  for (int col = 0; col < M; col++) {
    int current_length = 0;

    if (board[1][col] != 0) current_length++; //first occurence of player

    for (int row = 1; row < N; row++) {
      if (board[row][col] == 0) current_length = 0; //reset because no player
      else if (board[row-1][col] == board[row][col]) current_length++; //another occurence
      else current_length = 1; //first occurence of player

      if (current_length >= K) {
        *winner = board[row][col];
        *end_row = row+1;
        *end_col = col+1;
        *start_row = (row+1) - current_length + 1;
        *start_col = (col+1);
        //printf("curr_length: %d, winner = %d, start_row = %d, start_col = %d, end_row = %d, end_col = %d\n", current_length, winner, *start_row, *start_col, *end_row, *end_col);
        return;
      }
    }
  }

  *winner = 0;
  return;
}

void print_winner(void) {
  int winner;
  int start_row;
  int start_col;
  int end_row;
  int end_col;
  find_winner(&winner, &start_row, &start_col, &end_row, &end_col);
  if (winner == 0) return;
  printf("\n-------------------------------------\n");
  printf("Player %d has won the game, from (%.2d, %.2d) to (%.2d, %.2d)\n", winner-1, start_col, start_row, end_col, end_row);
  printf("-------------------------------------\n\n");
  printf("Clearing Board, starting new game:\n\n");
  // it does not start a new game correctly yet. It would have to start with q values first
  for (int row = 0; row < N; row++) for(int col = 0; col < M; col++) board[row][col] = 0;
  print_board();
}

int main(void) {
  bool player_2_turn = false;
  print_board();

  for (int q = 0; q < Q; q++) {
    get_next_entry(player_2_turn);
    print_board();
    print_winner();
  }
  player_2_turn = true;

  while (true) {
    for (int p = 0; p < P; p++) {
      get_next_entry(player_2_turn);
      print_board();
      print_winner();
    }
    player_2_turn = !player_2_turn;
  }
}

