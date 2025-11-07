/*
Tic-Tac-Toe Game (C17)

Features:
- Computer vs User gameplay
- Computer starts first ('O')
- User inputs position 1-9
- Detect win, draw, and show results

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

#include <corecrt_search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//  Constants
#define SIZE 3
#define EMPTY ' '
#define PLAYER_MARK 'X'
#define COMPUTER_MARK 'O'

//  Global board
static char board[SIZE][SIZE];

//  Function Declarations
static void initializeBoard(void);
static void displayBoard(void);
static int isBoardFull(void);
static int checkWin(char mark);
static void userMove(void);
static void computerMove(void);
static void makeMove(int position, char mark);
static int isValidMove(int position);
static void playGame(void);
static void announceResult(char winner);
static void clearInputBuffer(void);

//  Driver Code
int main(void) {
    srand((unsigned)time(NULL));
    printf("= Welcome to Tic-Tac-Toe! =\n");
    char playAgain;

    do {
        playGame();
        printf("\nPlay again? (y/n): ");
        clearInputBuffer();
        playAgain = getchar();
    } while (playAgain == 'y' || playAgain == 'Y');

    printf("\nThanks for playing!\n");
    return 0;
}

//  Initialize Board
static void initializeBoard(void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

//  Display Board
static void displayBoard(void) {
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf(" ");
        for (int j = 0; j < SIZE; j++) {
            char cell = board[i][j];
            if (cell == EMPTY)
                printf("%d", i * SIZE + j + 1);
            else
                printf("%c", cell);

            if (j < SIZE - 1)
                printf(" | ");
        }
        printf("\n");
        if (i < SIZE - 1)
            printf("---+---+---\n");
    }
    printf("\n");
}

//  Place mark on board
static void makeMove(int position, char mark) {
    int row = (position - 1) / SIZE;
    int col = (position - 1) % SIZE;
    board[row][col] = mark;
}

//  Check if a move is valid
static int isValidMove(int position) {
    if (position < 1 || position > 9)
        return 0;
    int row = (position - 1) / SIZE;
    int col = (position - 1) % SIZE;
    return board[row][col] == EMPTY;
}

//  Check if the board is full
static int isBoardFull(void) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == EMPTY)
                return 0;
    return 1;
}

//  Check If Player Has Won
static int checkWin(char mark) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) ||
            (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark))
            return 1;
    }
    // Check diagonals
    if ((board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) ||
        (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark))
        return 1;

    return 0;
}

//  Computer random move
static void computerMove(void) {
    int position;
    do {
        position = rand() % 9 + 1;
    } while (!isValidMove(position));
    printf("Computer chooses position %d.\n", position);
    makeMove(position, COMPUTER_MARK);
}

//  Player move
static void userMove(void) {
    int position;
    do {
        printf("Enter your move (1 -> 9): ");
        if (scanf_s("%d", &position) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (!isValidMove(position))
            printf("Invalid move. Try again.\n");
    } while (!isValidMove(position));
    makeMove(position, PLAYER_MARK);
}

//  Utility: Clear invalid input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//  Announce Result
static void announceResult( char winner) {
    if (winner == PLAYER_MARK)
        printf("\nYou win!\n");
    else if (winner == COMPUTER_MARK)
        printf("\nComputer wins!\n");
    else
        printf("\nIt's a draw!\n");
}

//  Play the game logic
static void playGame(void) {
    char winner = EMPTY;
    initializeBoard();
    printf("Computer: %c | Player: %c\n", COMPUTER_MARK, PLAYER_MARK);
    displayBoard();

    while (1) {
        computerMove();
        displayBoard();
        if (checkWin(COMPUTER_MARK)) {
            winner = COMPUTER_MARK;
            break;
        }
        if (isBoardFull()) break;

        userMove();
        displayBoard();
        if (checkWin(PLAYER_MARK)) {
            winner = PLAYER_MARK;
            break;
        }
        if (isBoardFull()) break;
    }
    announceResult(winner);
}