/*
Rock Paper Scissors Game (C17)

Description:
A simple terminal-based Rock Paper Scissors game.
Player vs Computer using characters:
s = Stone (Rock)
p = Paper
z = Scissors

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

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char STONE = 's';
const char PAPER = 'p';
const char SCISSORS = 'z';

//  Randomly generate computer choice
static char getComputerChoice(void) {
    int random_number = rand() % 3;
    switch (random_number) {
        case 0: return 's';     //  Stone
        case 1: return 'p';     //  Paper
        default: return 'z';    //  Scissors
    }
}

//  Determine the winner
static int determine_winner(char player, char computer) {
    if (player == computer)
        return 0;

    if ((player == 's' && computer == 'z') ||
        (player == 'p' && computer == 's') ||
        (player == 'z' && computer == 'p'))
        return 1;

    return -1;
}

//  Ultility to clear input buffer after invalid input
static void clearInputBuffer(void) {
    int clear;
    while ((clear = getchar()) != '\n' && clear != EOF) {
        ;   //  Discard characters
    }
}

//  Driver Code
int main(void) {
    char player_choice;
    char computer_choice;
    int result;

    printf("= Rock Paper Scissors Game =\n");
    printf("Use the following keys:\n");
    printf(" s = Stone (Rock)\n");
    printf(" p = Paper\n");
    printf(" z = Scissors\n\n");

    //  Seed random number generator
    srand((unsigned int)time(NULL));

    while (1) {
        printf("Enter your choice (s, p, z) or q to quit: ");
        if (scanf_s(" %c", &player_choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (player_choice == 'q' || player_choice == 'Q') {
            printf("Thanks for playing! Goodbye!\n");
            break;
        }

        if (player_choice != 's' && player_choice != 'p' && player_choice != 'z') {
            printf("Invalid choice! Please enter s, p, or z\n\n");
            continue;
        }

        computer_choice = getComputerChoice();
        printf("You chose: %c\n", player_choice);
        printf("Computer chose: %c\n", computer_choice);

        result = determine_winner(player_choice, computer_choice);

        if (result == 0)
            printf("It's a draw!\n\n");
        else if (result == 1) 
            printf("You win!\n\n");
        else
            printf("Computer wins!\n\n");
    }
    return 0;
}
