/*
Typing Tutor

Features:
- Display random words from a predefined list.
- Measure typing speed (words per minute).
- Check accuracy.
- Maintain a score.
- Multiple difficulty levels.

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
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

// Constants & Macros
#define MAX_WORD_LENGTH  32
#define MAX_WORDS        100
#define ROUND_WORDS      10

// Struct Definitions
typedef enum Difficulty {
    EASY,
    MEDIUM,
    HARD
} difficulty_t;


typedef struct Word {
    char text[MAX_WORD_LENGTH];
} word_t;

typedef struct TypingStats {
    int correct_words;
    int total_words;
    double elapsed_time;    // in seconds
} typing_stats_t;

// Global Variables
static word_t word_list[MAX_WORDS] = {
    {"apple"}, {"banana"}, {"computer"}, {"keyboard"}, {"program"},
    {"function"}, {"variable"}, {"pointer"}, {"string"}, {"compile"},
    {"syntax"}, {"debug"}, {"loop"}, {"array"}, {"stack"},
    {"queue"}, {"algorithm"}, {"memory"}, {"random"}, {"score"},
    {"game"}, {"player"}, {"input"}, {"output"}, {"file"},
    {"text"}, {"line"}, {"character"}, {"time"}, {"speed"},
    {"accuracy"}, {"typing"}, {"challenge"}, {"practice"}, {"lesson"},
    {"code"}, {"logic"}, {"condition"}, {"while"}, {"for"},
    {"if"}, {"else"}, {"switch"}, {"case"}, {"break"},
    {"continue"}, {"return"}, {"main"}, {"struct"}, {"define"},
    {"const"}, {"enum"}, {"header"}, {"function"}, {"variable"},
    {"int"}, {"float"}, {"double"}, {"char"}, {"bool"},
    {"true"}, {"false"}, {"null"}, {"pointer"}, {"array"},
    {"stack"}, {"queue"}, {"tree"}, {"graph"}, {"node"},
    {"algorithm"}, {"search"}, {"sort"}, {"insert"}, {"delete"},
    {"update"}, {"calculate"}, {"measure"}, {"performance"}, {"score"},
    {"challenge"}, {"practice"}, {"learn"}, {"master"}, {"improve"},
    {"skill"}, {"typing"}, {"fast"}, {"accurate"}, {"keyboard"},
    {"mouse"}, {"screen"}, {"display"}, {"input"}, {"output"},
    {"file"}, {"read"}, {"write"}, {"open"}, {"close"}
};

static difficulty_t game_difficulty = EASY;

// Function Declarations
static void startTypingTutor(void);
static void clearScreen(void);
static int randomIndex(int max);
static void calculateStats(const typing_stats_t *stats);
static void selectDifficulty(void);
static word_t getRandomWord(void);
static void clearInputBuffer(void);

//  Utility Functions - Clear console screen cross-platform
static void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
    
//  Utility Function - Clear leftover input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

//  Utility Function - Return a random index from 0 to (max-1)
static int randomIndex(int max) {
    return rand() % max;
}

//  Driver Code
int main(void) {
    printf("= TYPING TUTOR =\n");
    printf("Press any key to start...\n");
    _getch();

    startTypingTutor();

    printf("\nThanks for playing!\n");
    return EXIT_SUCCESS;
}

// Function Definitions
static void selectDifficulty(void) {
    int choice = 0;
    clearScreen();
    printf("Select Difficulty Level:\n");
    printf("1. EASY (common words)\n");
    printf("2. MEDIUM (slightly harder words)\n");
    printf("3. HARD (advanced words)\n");
    printf("Enter choice (1-3): ");

    while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice. Enter 1, 2, or 3: ");
        clearInputBuffer();
    }

    clearInputBuffer();
    game_difficulty = (difficulty_t)choice;
}

static word_t getRandomWord(void) {
    int start = 0, end = 0;

    switch (game_difficulty) {
        case EASY:   start = 0;   end = 33; break;
        case MEDIUM: start = 34;  end = 66; break;
        case HARD:   start = 67;  end = 99; break;
    }

    return word_list[start + randomIndex(end - start + 1)];
}

static void calculateStats(const typing_stats_t *stats) {
    double words_per_minute = (stats->total_words / stats->elapsed_time) * 60.0;
    double accuracy = ((double)stats->correct_words / stats->total_words) * 100.0;

    printf("\n= Typing Test Results =\n");
    printf("Total Words:   %d\n", stats->total_words);
    printf("Correct Words: %d\n", stats->correct_words);
    printf("Accuracy:      %.2f%%\n", accuracy);
    printf("Elapsed Time:  %.2f seconds\n", stats->elapsed_time);
    printf("Speed:         %.2f WPM\n", words_per_minute);
}

static void startTypingTutor(void) {
    typing_stats_t stats = {0, 0, 0.0};
    char input[MAX_WORD_LENGTH];
    clock_t start_time, end_time;

    selectDifficulty();
    clearScreen();

    srand((unsigned)time(NULL));
    start_time = clock();

    for (int i = 0; i < ROUND_WORDS; ++i) {
        word_t word = getRandomWord();
        printf("Word %d: %s\n", i + 1, word.text);
        printf("Your input: ");
        scanf_s("%31s", input, (unsigned)_countof(input));
        clearInputBuffer();

        stats.total_words++;
        if (_stricmp(input, word.text) == 0) {
            stats.correct_words++;
        }

        clearScreen();
    }

    end_time = clock();
    stats.elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    calculateStats(&stats);
}