/*
Pacman Game

Features:
- Ghost enemies (G) that move randomly.
- Pacman loses HP on collision.
- Game over when HP reaches 0.
- Dots still give points.

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
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

//  Constants & Macros
#define MAP_WIDTH   20
#define MAP_HEIGHT  10

#define WALL        '#'
#define EMPTY       ' '
#define PLAYER      'P'
#define GHOST       'G'
#define DOT         '.'

#define MAX_GHOSTS  3
#define PLAYER_HP   3

//  Struct Definitions
typedef struct Player {
    int x;
    int y;
    int score;
    int hp;
} player_t;

typedef struct Ghost {
    int x;
    int y;
    int damage;
} ghost_t;

//  Global Variables
static char game_map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "####################",
    "#.................#",
    "#.#####.#####.####",
    "#.................#",
    "#.###.#.###.#.####",
    "#.....#.....#.....#",
    "###.#.#####.#.###.#",
    "#.................#",
    "####################",
};

static player_t player = { 1, 1, 0, PLAYER_HP };
static ghost_t ghosts[MAX_GHOSTS];

//  Utility Functions
static void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
    
static bool dotsRemaining(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (game_map[y][x] == DOT)
                return true;
        }
    }
    return false;
}

// Function Declarations
static void renderGame(void);
static bool isValidMove(int y, int x);
static void movePlayer(char input);
static void moveGhosts(void);
static bool checkCollision(void);
static void gameLoop(void);
static void initGhosts(void);

//  Driver Code
int main(void) {
    printf("= PACMAN =\n");
    printf("Press any key to start...\n");
    _getch();

    initGhosts();
    gameLoop();

    return 0;
}


// Function Declarations
static void renderGame(void) {
    clearScreen();

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            bool ghost_here = false;
            for (int g = 0; g < MAX_GHOSTS; g++) {
                if (ghosts[g].x == x && ghosts[g].y == y) {
                    putchar(GHOST);
                    ghost_here = true;
                    break;
                }
            }
            if (ghost_here)
                continue;

            if (y == player.y && x == player.x)
                putchar(PLAYER);
            else
                putchar(game_map[y][x]);
        }
        putchar('\n');
    }

    printf("\nScore: %d   HP: %d\n", player.score, player.hp);
    printf("Use W/A/S/D to move, Q to quit.\n");
}

static bool isValidMove(int new_y, int new_x) {
    return (game_map[new_y][new_x] != WALL);
}

static void movePlayer(char input) {
    int new_x = player.x;
    int new_y = player.y;

    switch (toupper(input)) {
        case 'W': new_y--; break;
        case 'S': new_y++; break;
        case 'A': new_x--; break;
        case 'D': new_x++; break;
        default: return;
    }

    if (isValidMove(new_y, new_x)) {
        if (game_map[new_y][new_x] == DOT) {
            player.score += 10;
            game_map[new_y][new_x] = EMPTY;
        }
        player.x = new_x;
        player.y = new_y;
    }
}

static void moveGhosts(void) {
    for (int g = 0; g < MAX_GHOSTS; g++) {
        int direction = rand() % 4;
        int new_x = ghosts[g].x;
        int new_y = ghosts[g].y;

        switch (direction) {
            case 0: new_y--; break;
            case 1: new_y++; break;
            case 2: new_x--; break;
            case 3: new_x++; break;
        }

        if (isValidMove(new_y, new_x)) {
            ghosts[g].x = new_x;
            ghosts[g].y = new_y;
        }
    }
}

static bool checkCollision(void) {
    for (int g = 0; g < MAX_GHOSTS; g++) {
        if (ghosts[g].x == player.x && ghosts[g].y == player.y) {
            player.hp -= ghosts[g].damage;
            printf("Ouch! A ghost hit you! HP = %d\n", player.hp);
            _getch();
            if (player.hp <= 0)
                return true;
        }
    }
    return false;
}

static void initGhosts(void) {
    srand((unsigned)time(NULL));
    for (int g = 0; g < MAX_GHOSTS; ++g) {
        ghosts[g].x = 5 + g * 3;
        ghosts[g].y = 3 + g;
        ghosts[g].damage = 1;
    }
}

static void gameLoop(void) {
    renderGame();

    while (player.hp > 0) {
        if (_kbhit()) {
            char input = _getch();
            if (toupper(input) == 'Q')
                break;

            movePlayer(input);
        }

        moveGhosts();

        if (checkCollision())
            break;

        renderGame();

        if (!dotsRemaining()) {
            printf("\nCongratulations! You cleared all dots!\n");
            break;
        }

        Sleep(200);
    }

    clearScreen();
    if (player.hp > 0)
        printf("\nYou Win! Final Score: %d\n", player.score);
    else
        printf("\nGame Over! Final Score: %d\n", player.score);
}