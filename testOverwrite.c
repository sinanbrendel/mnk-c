#include <stdio.h>
#include <unistd.h>

int main() {
    // Print 20 lines
    for (int i = 1; i <= 20; i++) {
        printf("Line, %d\n", i);
    }

    sleep(3);
    // Move cursor up 20 lines
    printf("\033[20A");

    // Clear and rewrite those lines
    for (int i = 1; i <= 20; i++) {
        printf("\033[2K"); // clear current line
        printf("New line %d\n", i);
    }

    return 0;
}
