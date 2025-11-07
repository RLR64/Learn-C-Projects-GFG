/*
Snake and Ladders Game (C17)

Description:
Two-player Snake and Ladders game using dice rolls.
Demonstrates clean modular structure with static and const static.

For Linux/macOS:
Replace fopen_s, strcpy_s, etc., with fopen, strcpy, if needed.

Code Structure:
Includes
Constants & Macros  
Struct Definitions
Global Variables
Function Declarations
Driver Code (main)
Function Definitions
*/

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

//  Constants & Macros  
#define WINNING_POSITION 100

//  Global Variables
//  Read only ladders and snakes
 const static int ladders[][2] = {
    {4, 14}, {9, 31}, {20, 38}, {28, 84},
    {40, 59}, {63, 81}, {71, 91}
 };

 const static int snakes[][2] = {
    {17, 7}, {54, 34}, {62, 19}, {64, 60},
    {87, 24}, {93, 73}, {95, 75}, {99, 78}
};

//  Function Declarations
static int rollDice(void);
static int checkLadderOrSnake(int position);
static void printTurnInfo(int player, int roll, int position);
static void clearInputBuffer(void);
static int waitForRollOrQuit(int player);

//  Utility Function - Clear leftover input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//  Roll a dice (1 - 6)
static int rollDice(void) {
    return (rand() % 6) + 1;
}

//  Driver Code
int main(void) {
    int player1 = 0, player2 = 0;
    int currentPlayer = 1;
    int dice;

    srand((unsigned int)time(NULL));

    printf("= Snake and Ladders Game =\n");
    printf("First to reach %d wins!\n\n", WINNING_POSITION);

    while (1) {
        if (!waitForRollOrQuit(currentPlayer)) {
            printf("\nPlayer %d quit the game. Goodbye!\n", currentPlayer);
            break;
        }

        dice = rollDice();

        if (currentPlayer == 1) {
            player1 += dice;
            if (player1 > WINNING_POSITION) player1 = WINNING_POSITION;
            player1 = checkLadderOrSnake(player1);
            printTurnInfo(1, dice, player1);

            if (player1 >= WINNING_POSITION) {
                printf("\nPlayer 1 wins!\n");
                break;
            }

            currentPlayer = 2;

        } else {
            player2 += dice;
            if (player2 > WINNING_POSITION) player2 = WINNING_POSITION;
            player2 = checkLadderOrSnake(player2);
            printTurnInfo(2, dice, player2);

            if (player2 >= WINNING_POSITION) {
                printf("\nPlayer 2 wins!\n");
                break;
            }

            currentPlayer = 1;
        }

        printf("\n---------------------------------\n");
    }
    printf("Game Over!\n");
    return 0;
}

//  Adjust player position for snakes or ladders
static int checkLadderOrSnake(int position) {
    for (size_t i = 0; i < sizeof(ladders) / sizeof(ladders[0]); i++) {
        if (position == ladders[i][0]) {
            printf("Ladder! Climb up from %d to %d!\n", ladders[i][0], ladders[i][1]);
            return ladders[i][1];
        }
    }
    for (size_t i = 0; i < sizeof(snakes) / sizeof(snakes[0]); i++) {
        if (position == snakes[i][0]) {
            printf("Snake! Slide down from %d to %d!\n", snakes[i][0], snakes[i][1]);
            return snakes[i][1];
        }
    }
    return position;
}

//  Display current turn info
static void printTurnInfo(int player, int roll, int position) {
    printf("Player %d rolled a %d now at position %d\n", player, roll, position);
}

//  Wait for ENTER or 'q' to quit
static int waitForRollOrQuit(int player) {
    printf("Player %d, Press ENTER to roll the dice or 'q' to quit: ", player);
    int ch = getchar();

    if (ch == 'q' || ch == 'Q') {
        return 0;
    }

    clearInputBuffer();
    return 1;

}