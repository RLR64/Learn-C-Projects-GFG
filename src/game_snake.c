/*
Snake Game

Features:
- Display a rectangular game board.
- Move snake using W/A/S/D keys.
- Eat food to grow the snake.
- Game over if the snake hits itself or the walls.

For Linux/macOS:
Replace fopen_s, strcpy_s, etc., with fopen, strcpy, if needed.

Code Structure:
1. Includes              → Documentation Section
2. Constants & Macros    → Link/ File Inclusion Section
3. Struct/Enum Defs      → Definition Section
4. Global Variables      → Global Declaration Section
5. Function Declarations → Forward declarations (use 'static' internally)
6. int main(void)        → main(void) Function Section
7. Function Definitions  → Subprogram Section
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // _kbhit() and _getch()
#include <stdbool.h>
#include <time.h>
#include <windows.h>    // Sleep()

// Constants & Macros
#define MAP_WIDTH           30
#define MAP_HEIGHT          15
#define SNAKE_SYMBOL        'O'
#define FOOD_SYMBOL         '*'
#define EMPTY_SPACE         ' '
#define INITIAL_SNAKE_LENGTH 3
#define HIGH_SCORE_FILE     "highscore.txt"

// Struct & Enum Definitions
typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct SnakeNode {
    int x, y;
    struct SnakeNode *next;
} snake_node_t;

typedef struct Snake {
    snake_node_t *head;
    snake_node_t *tail;
    direction_t dir;
    int length;
} snake_t;

typedef struct Food {
    int x, y;
} food_t;

// Global Variables
static snake_t snake;
static food_t food;
static bool game_over = false;
static int score = 0;
static int high_score = 0;

// Function Declarations

// Memory Management
static void cleanupSnake(void);

// Game Logic
static void initializeGame(void);
static void updateGame(void);
static void growSnake(void);
static bool isCollision(int x, int y);

// Food Management
static void placeFood(void);

// Input & Rendering
static void handleInput(void);
static void renderGame(void);

// Utilities
static void clearScreen(void);
static void loadHighScore(void);
static void saveHighScore(void);

// Utilities
static void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

    static void loadHighScore(void) {
        FILE *file = NULL;
        errno_t error = fopen_s(&file, HIGH_SCORE_FILE, "r");
    
        if (error == 0 && file != NULL) {
            fscanf_s(file, "%d", &high_score);
            fclose(file);
        }
}
    
    static void saveHighScore(void) {
        FILE *file = NULL;
        errno_t error = fopen_s(&file, HIGH_SCORE_FILE, "w");
    
        if (error == 0 && file != NULL) {
            fprintf(file, "%d", high_score);  // Fixed: was fscanf_s
            fclose(file);
        }
}

// Driver Code
int main(void) {
    printf("= SNAKE GAME (Dynamic Linked List) =\n");
    printf("Press any key to start...\n");
    _getch();

    initializeGame();

    while (!game_over) {
        handleInput();
        updateGame();
        renderGame();
        Sleep(150);
    }

    printf("\nGame Over! Final Score: %d  Snake Length: %d\n", score, snake.length);
    
    if (score > high_score) {
        high_score = score;
        saveHighScore();
        printf("New High Score!\n");
    }

    cleanupSnake();
    return 0;
}

// Function Definitions
// Memory Management
static void cleanupSnake(void) {
    snake_node_t *curr = snake.head;
    while (curr) {
        snake_node_t *temp = curr;
        curr = curr->next;
        free(temp);
    }
    snake.head = NULL;
    snake.tail = NULL;
}

// Game Logic
static void initializeGame(void) {
    // Initialize snake
    snake.head = malloc(sizeof(snake_node_t));
    if (!snake.head) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    snake.head->x = 5;
    snake.head->y = 5;
    snake.head->next = NULL;
    snake.tail = snake.head;
    snake.length = 1;
    snake.dir = RIGHT;

    // Grow initial snake
    for (int i = 1; i < INITIAL_SNAKE_LENGTH; i++)
        growSnake();

    score = 0;
    game_over = false;
    srand((unsigned)time(NULL));
    loadHighScore();
    placeFood();
}

static void updateGame(void) {
    // Move snake: create new head
    snake_node_t *new_head = malloc(sizeof(snake_node_t));
    if (!new_head) {
        game_over = true;
        return;
    }
    
    new_head->x = snake.head->x;
    new_head->y = snake.head->y;
    new_head->next = snake.head;
    snake.head = new_head;

    switch (snake.dir) {
        case UP:    snake.head->y--; break;
        case DOWN:  snake.head->y++; break;
        case LEFT:  snake.head->x--; break;
        case RIGHT: snake.head->x++; break;
    }

    // Check collisions
    if (isCollision(snake.head->x, snake.head->y)) {
        game_over = true;
        return;
    }

    // Check food
    if (snake.head->x == food.x && snake.head->y == food.y) {
        score += 10;
        placeFood();
        growSnake();
    } else {
        // Remove tail
        snake_node_t *prev = NULL, *curr = snake.head;
        while (curr->next) {
            prev = curr;
            curr = curr->next;
        }
        if (prev) {
            prev->next = NULL;
            free(curr);
        }
    }
}

static void growSnake(void) {
    snake_node_t *new_node = malloc(sizeof(snake_node_t));
    if (!new_node) {
        game_over = true;
        return;
    }
    
    new_node->x = snake.tail->x;
    new_node->y = snake.tail->y;
    new_node->next = NULL;
    snake.tail->next = new_node;
    snake.tail = new_node;
    snake.length++;
}

static bool isCollision(int x, int y) {
    // Check walls
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;

    // Check snake body
    snake_node_t *curr = snake.head;
    while (curr) {
        if (curr->x == x && curr->y == y)
            return true;
        curr = curr->next;
    }
    return false;
}

// Food Management
static void placeFood(void) {
    while (true) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;

        if (!isCollision(x, y)) {
            food.x = x;
            food.y = y;
            break;
        }
    }
}

// Input & Rendering
static void handleInput(void) {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w': case 'W': if (snake.dir != DOWN)  snake.dir = UP; break;
            case 's': case 'S': if (snake.dir != UP)    snake.dir = DOWN; break;
            case 'a': case 'A': if (snake.dir != RIGHT) snake.dir = LEFT; break;
            case 'd': case 'D': if (snake.dir != LEFT)  snake.dir = RIGHT; break;
            case 'q': case 'Q': game_over = true; break;
        }
    }
}

static void renderGame(void) {
    clearScreen();
    
    // Print score at top (reduces flicker)
    printf("Score: %d  Length: %d  High Score: %d\n", score, snake.length, high_score);
    printf("Use W/A/S/D to move, Q to quit.\n\n");

    // Render game board
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (x == food.x && y == food.y) {
                putchar(FOOD_SYMBOL);
            } else {
                bool printed = false;
                snake_node_t *curr = snake.head;
                while (curr) {
                    if (curr->x == x && curr->y == y) {
                        putchar(SNAKE_SYMBOL);
                        printed = true;
                        break;
                    }
                    curr = curr->next;
                }
                if (!printed)
                    putchar(EMPTY_SPACE);
            }
        }
        putchar('\n');
    }
}