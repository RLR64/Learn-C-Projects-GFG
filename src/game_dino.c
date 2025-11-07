/*
Dino Game
Text-based Chrome offline Dino Game clone in C17.

Features:
- Dino can jump over obstacles (cacti, rocks)
- Obstacles move from right to left
- Score increases over time
- Collision causes damage/game over

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

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // For _kbhit() and _getch()
#include <stdbool.h>
#include <time.h>
#include <windows.h>    // For Sleep() on Windows

// Constants & Macros
#define SCREEN_WIDTH        50
#define SCREEN_HEIGHT       10
#define GROUND_Y            (SCREEN_HEIGHT - 2)
#define MAX_OBSTACLES       10

#define DINO_SYMBOL           'D'
#define OBSTACLE_SYMBOL_SMALL 'w'
#define OBSTACLE_SYMBOL_LARGE 'Y'
#define OBSTACLE_SYMBOL_ROCK  'o'

// Struct Definitions
typedef struct Dino {
    int x;
    int y;
    int velocity;       // Jump velocity
    bool isJumping;
    int hp;
} dino_t;

typedef struct Obstacle {
    int x;
    int y;
    int damage;
    char symbol;
    bool active;
} obstacle_t;

// Global Variables
static dino_t player;
static obstacle_t obstacles[MAX_OBSTACLES];
static int score = 0;
static int top_score = 0;

// Function Declarations
static void initializeGame(void);
static void renderGame(void);
static void updateGame(void);
static void spawnObstacle(void);
static void moveObstacles(void);
static void checkCollision(void);
static void handleInput(void);
static void clearScreen(void);
static void loadTopScore(void);
static void saveTopScore(void);

// Utility Functions - Clears the terminal/console screen in a cross-platform way.
static void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void loadTopScore(void) {
    FILE *file = NULL;
    errno_t error = fopen_s(&file, "top_score.txt", "r");

    if (error == 0 && file != NULL) {
        fscanf_s(file, "%d", &top_score);
        fclose(file);
    }

}

static void saveTopScore(void) {
    FILE *file = NULL;
    errno_t error =  fopen_s(&file, "top_score.txt", "w");

    if (error == 0 && file != NULL) {
        fscanf_s(file, "%d", top_score);
        fclose(file);
    }
}

// Driver Code
int main(void) {
    printf("DINO GAME - Multiple Obstacles & Top Score\n");
    printf("Press any key to start...\n");
    _getch();

    initializeGame();

    while (player.hp > 0) {
        handleInput();
        updateGame();
        renderGame();
        Sleep(100);
    }

    printf("\nGame Over! Final Score: %d\n", score);
    if (score > top_score) {
        top_score = score;
        saveTopScore();
    }
    printf("Top Score: %d\n", top_score);

    return 0;
}

// Function Definitions
static void initializeGame(void) {
    player.x = 5;
    player.y = GROUND_Y;
    player.velocity = 0;
    player.isJumping = false;
    player.hp = 1;

    score = 0;

    for (int i = 0; i < MAX_OBSTACLES; i++)
        obstacles[i].active = false;

    srand((unsigned)time(NULL));
    loadTopScore();
}

static void handleInput(void) {
    if (_kbhit()) {
        char key = _getch();
        if ((key == ' ' || key == 'w' || key == 'W') && !player.isJumping) {
            player.isJumping = true;
            player.velocity = -3;
        }
    }
}

static void updateGame(void) {
    // Update Dino Y position
    if (player.isJumping) {
        player.y += player.velocity;
        player.velocity += 1; // Gravity
        if (player.y >= GROUND_Y) {
            player.y = GROUND_Y;
            player.isJumping = false;
            player.velocity = 0;
        }
    }

    spawnObstacle();
    moveObstacles();
    checkCollision();
    score++;
}

static void spawnObstacle(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active && (rand() % 20 == 0)) {
            obstacles[i].active = true;
            obstacles[i].x = SCREEN_WIDTH - 1;
            obstacles[i].y = GROUND_Y;

            int type = rand() % 3;
            switch (type) {
                case 0: obstacles[i].symbol = OBSTACLE_SYMBOL_SMALL; obstacles[i].damage = 1; break;
                case 1: obstacles[i].symbol = OBSTACLE_SYMBOL_LARGE; obstacles[i].damage = 2; break;
                case 2: obstacles[i].symbol = OBSTACLE_SYMBOL_ROCK;  obstacles[i].damage = 1; break;
            }
            break;
        }
    }
}

static void moveObstacles(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            obstacles[i].x--;
            if (obstacles[i].x < 0)
                obstacles[i].active = false;
        }
    }
}

static void checkCollision(void) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active && obstacles[i].x == player.x && obstacles[i].y == player.y) {
            player.hp -= obstacles[i].damage;
            obstacles[i].active = false;
        }
    }
}

static void renderGame(void) {
    clearScreen();

    // Print score/HP at the top ONCE before the game area
    printf("Score: %d  HP: %d  Top Score: %d\n", score, player.hp, top_score);
    printf("Press SPACE to jump. Avoid obstacles!\n\n");

    // Render game area
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (y == player.y && x == player.x) {
                putchar(DINO_SYMBOL);
            } else {
                bool drawn = false;
                for (int i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].active && obstacles[i].x == x && obstacles[i].y == y) {
                        putchar(obstacles[i].symbol);
                        drawn = true;
                        break;
                    }
                }
                if (!drawn)
                    putchar((y == GROUND_Y) ? '-' : ' ');
            }
        }
        putchar('\n');
    }
}