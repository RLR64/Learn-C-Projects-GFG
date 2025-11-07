/*
2048 Game in C

Features:
- 4x4 grid
- Merge tiles of same value
- Insert new random tiles (2 or 4)
- Track score and high score
- Game over detection

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

//  Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

//  Constrants & Macros
#define GRID_SIZE 4
#define WIN_VALUE 2048

//  Global Variables
static int grid[GRID_SIZE][GRID_SIZE];
static int score = 0;
static int top_score = 0;

// Function Declarations
static void initializeGame(void);
static void renderGrid(void);
static void insertRandomTile(void);
static bool moveLeft(void);
static bool moveRight(void);
static bool moveUp(void);
static bool moveDown(void);
static bool canMove(void);
static void rotateGridClockwise(void);
static void copyGrid(int src[GRID_SIZE][GRID_SIZE], int dest[GRID_SIZE][GRID_SIZE]);
static bool gridsEqual(int a[GRID_SIZE][GRID_SIZE], int b[GRID_SIZE][GRID_SIZE]);
static void gameLoop(void);
static void clearScreen(void);

// Utility Functions
static void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Driver Code
int main(void) {
    printf("= 2048 GAME = \n");
    printf("Press any key to start...\n");
    _getch();

    initializeGame();
    gameLoop();

    printf("\nThanks for playing!\n");
    return 0;
}

//  Function Definitions
static void initializeGame(void) {
    score = 0;
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = 0;

    srand((unsigned)time(NULL));
    insertRandomTile();
    insertRandomTile();
}

static void insertRandomTile(void) {
    int empty_count = 0;
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (grid[i][j] == 0) empty_count++;

    if (empty_count == 0) return;

    int pos = rand() % empty_count;
    int value = (rand() % 10 == 0) ? 4 : 2;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) {
                if (pos == 0) {
                    grid[i][j] = value;
                    return;
                }
                pos--;
            }
        }
    }
}

static void renderGrid(void) {
    clearScreen();
    printf("2048 GAME\n");
    printf("Score: %d  Top Score: %d\n\n", score, top_score);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++)
            printf(grid[i][j] == 0 ? ".\t" : "%d\t", grid[i][j]);
        printf("\n");
    }

    printf("\nUse W/A/S/D to move, Q to quit.\n");
}

static bool moveLeft(void) {
    bool changed = false;
    for (int i = 0; i < GRID_SIZE; i++) {
        int last_merge = -1;
        for (int j = 1; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) continue;
            int k = j;
            while (k > 0 && grid[i][k - 1] == 0) {
                grid[i][k - 1] = grid[i][k];
                grid[i][k] = 0;
                k--;
                changed = true;
            }
            if (k > 0 && grid[i][k - 1] == grid[i][k] && last_merge != k - 1) {
                grid[i][k - 1] *= 2;
                score += grid[i][k - 1];
                grid[i][k] = 0;
                last_merge = k - 1;
                changed = true;
            }
        }
    }
    return changed;
}

static void rotateGridClockwise(void) {
    int tmp[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            tmp[j][GRID_SIZE - 1 - i] = grid[i][j];
    copyGrid(tmp, grid);
}

static bool moveRight(void) {
    rotateGridClockwise();
    rotateGridClockwise();
    bool c = moveLeft();
    rotateGridClockwise();
    rotateGridClockwise();
    return c;
}

static bool moveUp(void) {
    rotateGridClockwise();
    rotateGridClockwise();
    rotateGridClockwise();
    bool c = moveLeft();
    rotateGridClockwise();
    return c;
}

static bool moveDown(void) {
    rotateGridClockwise();
    bool c = moveLeft();
    rotateGridClockwise();
    rotateGridClockwise();
    rotateGridClockwise();
    return c;
}

static bool canMove(void) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (grid[i][j] == 0)
                return true;

    int tmp[GRID_SIZE][GRID_SIZE];
    int backup[GRID_SIZE][GRID_SIZE];
    copyGrid(grid, tmp);
    copyGrid(grid, backup);

    bool possible = false;

    moveLeft();
    if (!gridsEqual(grid, tmp)) possible = true;
    copyGrid(backup, grid);

    moveRight();
    if (!gridsEqual(grid, tmp)) possible = true;
    copyGrid(backup, grid);

    moveUp();
    if (!gridsEqual(grid, tmp)) possible = true;
    copyGrid(backup, grid);

    moveDown();
    if (!gridsEqual(grid, tmp)) possible = true;
    copyGrid(backup, grid);

    return possible;
}

static void copyGrid(int src[GRID_SIZE][GRID_SIZE], int dest[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            dest[i][j] = src[i][j];
}

static bool gridsEqual(int a[GRID_SIZE][GRID_SIZE], int b[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}

//  Game Loop
static void gameLoop(void) {
    renderGrid();
    while (true) {
        char input = _getch();
        bool moved = false;

        if (input == 'q' || input == 'Q')
            break;

        switch (input) {
            case 'w': case 'W': moved = moveUp(); break;
            case 'a': case 'A': moved = moveLeft(); break;
            case 's': case 'S': moved = moveDown(); break;
            case 'd': case 'D': moved = moveRight(); break;
        }

        if (moved) {
            insertRandomTile();
            if (score > top_score)
                top_score = score;
        }

        renderGrid();

        if (!canMove()) {
            printf("\nGame Over! Final Score: %d\n", score);
            break;
        }
    }
}