#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define M 20
#define N 20
#define K 6
#define P 2
#define Q 1
#define INPUT_SIZE 20

#define BOARD_LINE 0

uint_least8_t board[M][N] = {};
int line = 0;

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
  int start_line = line;

  do {
    printf("Player: %d\n", player);
    line++;
    printf("Enter x-position: ");
    x_pos = get_input() - 1;
    line++;
    printf("Enter y-position: ");
    y_pos = get_input() - 1;
    line++;
    printf("\n");
    line++;
    for (int i = 0; i < line-start_line; i++) {
      printf("\033[1A");
      printf("\033[2K");
    }
    line-=line-start_line;
    //printf("\033[%dA", line-start_line);
  } while (!validate_coord(x_pos, y_pos));



  board[y_pos][x_pos] = player + 1;
  return true;
}

void print_board(void) {
  const char player_1 = 'X', player_2 = 'O';

  printf("\033[%dA", line-BOARD_LINE);
  line = BOARD_LINE;
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
    line++;

    if (row < 1 || row > N - 1) continue; // skip horizontal separator
    for (int col = 0; col < M; col++) {
      if (col == 0) printf("    "); // offset to match numbered rows
      printf("---");
      if (col < M - 1) printf("+");
    }
    printf("\n");
    line++;
  }
  printf("\n");
  line++;
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

  //check "forward slash" diagonals (/), if long enough
  //forward slash starting at i'th row has i elements, row>0 caps iterated columns at 1
  //forward slash starting at n'th row has n elements, row>0 caps iterated columns at n
  //forward slash starting at n+1'th "row" has n-1 elements, column starts at 1+1=2=n+1-n+1
  //forward slash starting at n+n'th "row" has n-n=0 elements, column starts at 1+n
  //so we need to check slashes starting between rows K and N+N-1, columns start at row-n+1
  //4, 10-(4-1) = 10-3=7
  for (int row_0 = K-1; row_0 < 2*N-1; row_0++) {
    int col_start = 0;
    int row_start = row_0;
    if (row_0 > N) {
      col_start = row_0-N+1;
      row_start = N;
    }

    //printf("row_0: %d", row_0);
    int current_length = 0;

    if (board[row_start][0] != 0) current_length++; //first occurence of player
    
    //we now have to increment columns and decrement rows at the same time

    for (int row = row_0, col = col_start; row >= 0 || col < M; row--, col++) {
      if (board[row][col] == 0) current_length = 0; //reset because no player
      else if (board[row+1][col-1] == board[row][col]) current_length++; //another occurence
      else current_length = 1; //first occurence of player
      //printf("row_0: %d, row: %d, col: %d, curr_len: %d\n", row_0, row, col, current_length);

      if (current_length >= K) {
        *winner = board[row][col];
        *end_row = row+1;
        *end_col = col+1;
        *start_row = (row+1) + current_length - 1;
        *start_col = (col+1) - current_length + 1;
        //printf("curr_length: %d, winner = %d, start_row = %d, start_col = %d, end_row = %d, end_col = %d\n", current_length, winner, *start_row, *start_col, *end_row, *end_col);
        return;
      }
    }
  }

  //check "backward slash" diagonals (\)
  //the right way? 
  //1. set col = 1, row = N;
  //2. check diagonal beginning at (col, row);
  //3. if row=1: col++ goto 5;
  //4. else: row--; goto 2;
  //5. if col>M: return;
  //6. check diagonal beginning at (col, row);
  //7. col++; goto 5;
  for (int row_start = N-1; row_start >= 0; row_start--) {
    int col_start = 1;

    //DUPLICATE CODE -- START
    int current_length = 0;

    if (board[row_start][col_start] != 0) current_length++; //first occurence of player
    
    //we now have to increment columns and decrement rows at the same time

    for (int row = row_start, col = col_start; row < N || col < M; row++, col++) {
      if (board[row][col] == 0) current_length = 0; //reset because no player
      else if (board[row-1][col-1] == board[row][col]) current_length++; //another occurence
      else current_length = 1; //first occurence of player
      //printf("row_0: %d, row: %d, col: %d, curr_len: %d\n", row_0, row, col, current_length);

      if (current_length >= K) {
        *winner = board[row][col];
        *end_row = row+1;
        *end_col = col+1;
        *start_row = (row+1) - current_length + 1;
        *start_col = (col+1) - current_length + 1;
        //printf("curr_length: %d, winner = %d, start_row = %d, start_col = %d, end_row = %d, end_col = %d\n", current_length, winner, *start_row, *start_col, *end_row, *end_col);
        return;
      }
    }
    //DUPLICATE CODE -- END

  }
  for (int col_start = 2; col_start < M; col_start++) {
    int row_start = 1;

    //DUPLICATE CODE -- START
    int current_length = 0;

    if (board[row_start][col_start] != 0) current_length++; //first occurence of player
    
    //we now have to increment columns and decrement rows at the same time

    for (int row = row_start, col = col_start; row < N || col < M; row++, col++) {
      if (board[row][col] == 0) current_length = 0; //reset because no player
      else if (board[row-1][col-1] == board[row][col]) current_length++; //another occurence
      else current_length = 1; //first occurence of player
      //printf("row_0: %d, row: %d, col: %d, curr_len: %d\n", row_0, row, col, current_length);

      if (current_length >= K) {
        *winner = board[row][col];
        *end_row = row+1;
        *end_col = col+1;
        *start_row = (row+1) - current_length + 1;
        *start_col = (col+1) - current_length + 1;
        //printf("curr_length: %d, winner = %d, start_row = %d, start_col = %d, end_row = %d, end_col = %d\n", current_length, winner, *start_row, *start_col, *end_row, *end_col);
        return;
      }
    }
    //DUPLICATE CODE -- END
  }
  for (int row_0 = K-1; row_0 < 2*N-1; row_0++) {
    int col_start = 0;
    int row_start = row_0;
    if (row_0 > N) {
      col_start = row_0-N+1;
      row_start = N;
    }

    //printf("row_0: %d", row_0);
  }

  *winner = 0;
  return;
}

void print_winner(bool* restart) {
  int winner;
  int start_row;
  int start_col;
  int end_row;
  int end_col;
  find_winner(&winner, &start_row, &start_col, &end_row, &end_col);
  if (winner == 0) return;

  int start_line = line;

  printf("-------------------------------------\n");
  line++;
  printf("Player %d has won the game, from (%.2d, %.2d) to (%.2d, %.2d)\n", winner-1, start_col, start_row, end_col, end_row);
  line++;
  printf("-------------------------------------\n\n");
  line+=2;
  printf("Clearing Board, starting new game: ");
  fflush(stdout);
  *restart = true;

  for (int i = 0; i < 3; i++) {
    printf(".");
    fflush(stdout);
    sleep(1);
/*    printf("\033[3D"); //3 characters to the left
    printf("   ");
    printf("\033[3D"); //3 characters to the left
    */
  }
  printf("\n");
  line++;

  for (int i = 0; i < line-start_line; i++) {
    printf("\033[1A");
    printf("\033[2K");
  }
  line-=line-start_line;



  print_board();
  // it does not start a new game correctly yet. It would have to start with q values first
  for (int row = 0; row < N; row++) for(int col = 0; col < M; col++) board[row][col] = 0;
  print_board();
}

int main(void) {
  bool player_2_turn = false;
  bool restart = true;
  int line = 0;
  print_board();

  while (true) {
    if (restart) {
      restart = false;
      for (int q = 0; q < Q; q++) {
        if (restart) continue;
        get_next_entry(player_2_turn);
        print_board();
        print_winner(&restart);
      }
    } else {
      for (int p = 0; p < P; p++) {
        if (restart) continue;
        get_next_entry(player_2_turn);
        print_board();
        print_winner(&restart);
      }
    }
    player_2_turn = !player_2_turn;
  }
}

