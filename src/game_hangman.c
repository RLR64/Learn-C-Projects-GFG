/*
Hangman Game

Description:
Classic hangman word-guessing game implemented in C17,
following a compiler-friendly project layout. Demonstrates
structured design, modular static functions, and safe input handling.

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

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//  Constants & Macros
#define MAX_WORD_LENGTH 64
#define MAX_TRIES 6

//  Struct Definitions
typedef struct WordHint {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
} wordhint_t;

//  Global Variables
static const char *hangman_stages[MAX_TRIES + 1] = {
    "     _________\n"
    "    |         |\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n"
    "    |         |\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n"
    "    |        /|\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n"
    "    |        /|\\\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n"
    "    |        /|\\\n"
    "    |        /\n",

    "     _________\n"
    "    |         |\n"
    "    |         O\n"
    "    |        /|\\\n"
    "    |        / \\\n"
};

// 6. Function Declarations
static void display_word(const char *secret, const bool guessed[26]);
static void draw_hangman(int wrong_guesses);
static void clear_input_buffer(void);
static bool update_guess(const char *secret, char guess, char *current);
static bool all_letters_guessed(const char *secret, const char *current);

//  Driver Code
int main(void) {
    srand((unsigned)time(NULL));

    wordhint_t word_list[] = {
        {"geeksforgeeks", "Computer coding"},
        {"elephant", "A large mammal with a trunk"},
        {"pizza", "A popular Italian dish"},
        {"beach", "Sandy shore by the sea"},
        {"keyboard", "Input device with keys"},
        {"hangman", "This very game!"}
    };

    const size_t word_count = sizeof(word_list) / sizeof(word_list[0]);
    const size_t idx = (size_t)(rand() % word_count);
    const char *secret = word_list[idx].word;
    const char *hint = word_list[idx].hint;

    const size_t length = strlen(secret);
    if (length + 1 > MAX_WORD_LENGTH) {
        fprintf(stderr, "[ERROR]: Selected word too long!\n");
        return EXIT_FAILURE;
    }

    bool guessed_letters[26] = {false};
    char current[MAX_WORD_LENGTH + 1] = {0};
    memset(current, '_', length);
    current[length] = '\0';

    printf("= Welcome to Hangman! =\n");
    printf("Hint: %s\n\n", hint);

    int wrong_guesses = 0;
    bool game_won = false;

    while (wrong_guesses < MAX_TRIES && !game_won) {
        display_word(secret, guessed_letters);
        draw_hangman(wrong_guesses);

        printf("Enter a letter guess: ");
        int input = getchar();

        if (input == EOF) break;
        if (input == '\n') continue;

        char guess = (char)tolower(input);
        clear_input_buffer();

        if (guess < 'a' || guess > 'z') {
            printf("Invalid input. Please enter a lowercase letter.\n\n");
            continue;
        }

        if (guessed_letters[guess - 'a']) {
            printf("You already guessed '%c'. Try another.\n\n", guess);
            continue;
        }

        guessed_letters[guess - 'a'] = true;
        bool hit = update_guess(secret, guess, current);

        if (hit) {
            printf("Good guess! '%c' is in the word.\n\n", guess);
        } else {
            printf("Wrong guess '%c'.\n\n", guess);
            wrong_guesses++;
        }

        game_won = all_letters_guessed(secret, current);
    }

    if (game_won) {
        printf("Congratulations! You guessed the word: %s\n", secret);
    } else {
        draw_hangman(MAX_TRIES);
        printf("Game Over! The word was: %s\n", secret);
    }

    return EXIT_SUCCESS;
}

// Function Definitions
// Display current word progress with underscores and guessed letters
static void display_word(const char *secret, const bool guessed[26]) {
    printf("Word: ");
    for (size_t i = 0; secret[i] != '\0'; i++) {
        char ch = secret[i];
        if (guessed[ch - 'a']) {
            printf("%c ", ch);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// Draw hangman stage corresponding to number of wrong guesses
static void draw_hangman(int wrong_guesses) {
    if (wrong_guesses < 0) wrong_guesses = 0;
    if (wrong_guesses > MAX_TRIES) wrong_guesses = MAX_TRIES;
    printf("%s\n", hangman_stages[wrong_guesses]);
}

// Clear remaining characters from stdin until newline or EOF
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Update guessed word state; returns true if the guess is correct
static bool update_guess(const char *secret, char guess, char *current) {
    bool hit = false;
    for (size_t i = 0; secret[i] != '\0'; i++) {
        if (secret[i] == guess) {
            current[i] = guess;
            hit = true;
        }
    }
    return hit;
}

// Check if all letters have been guessed
static bool all_letters_guessed(const char *secret, const char *current) {
    return strcmp(secret, current) == 0;
}
