/*
Quiz Game Project (C17)

Features:
- Insert questions
- Check answers
- Calculate score

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

#include <stdio.h>
#include <string.h>

// Constants & Macros
#define MAX_QUESTIONS 10
#define MAX_OPTIONS 4
#define MAX_TEXT_LENGTH 256
#define QUIZ_FILE "quiz_data.dat"

// Struct Definitions
typedef struct {
    char question[MAX_TEXT_LENGTH];
    char options[MAX_OPTIONS][MAX_TEXT_LENGTH];
    int correct_option;   // 1-based index (1..4)
} question_t;

// Global Variables
static question_t quiz[MAX_QUESTIONS];
static int questionCount = 0;

// Function Declarations
static void insertQuestion(const char *question_text,
                            const char *option1,
                            const char *option2,
                            const char *option3,
                            const char *option4,
                            int correct_option);

static void display_question(const question_t *question, int index);
static int check_answer(const question_t *question, int user_choice);
static void play_quiz(void);
static void show_score(int score, int total);

// New File I/O functions
static void save_questions_to_file(const char *filename);
static void load_questions_from_file(const char *filename);

// Utility Functions
static void insertQuestion(const char *question_text,
                            const char *option1,
                            const char *option2,
                            const char *option3,
                            const char *option4,
                            int correct_option) {
    if (questionCount >= MAX_QUESTIONS) {
        printf("Cannot insert more questions.\n");
        return;
    }

    question_t *question = &quiz[questionCount++];
    strcpy_s(question->question, sizeof(question->question), question_text);
    strcpy_s(question->options[0], sizeof(question->options[0]), option1);
    strcpy_s(question->options[1], sizeof(question->options[1]), option2);
    strcpy_s(question->options[2], sizeof(question->options[2]), option3);
    strcpy_s(question->options[3], sizeof(question->options[3]), option4);
    question->correct_option = correct_option;
}

// Driver Code
int main(void) {
    printf("Welcome to the Quiz Game!\n");

    // Step 1: Load previous quiz data
    load_questions_from_file(QUIZ_FILE);

    // Step 2: If no data, insert default questions
    if (questionCount == 0) {
        insertQuestion("What is the capital of France?",
                        "Berlin", "Madrid", "Paris", "Lisbon", 3);
        insertQuestion("Which language is used for system programming?",
                        "Python", "C", "JavaScript", "HTML", 2);
        insertQuestion("What is 2 + 2 * 2?",
                        "6", "8", "4", "10", 1);

        save_questions_to_file(QUIZ_FILE);
    }

    // Step 3: Play quiz
    play_quiz();

    return 0;
}

// Function Declarations
static void save_questions_to_file(const char *filename) {
    FILE *file = NULL;
    errno_t err = fopen_s(&file, filename, "wb");
    if (err != 0 || !file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fwrite(&questionCount, sizeof(int), 1, file);
    fwrite(quiz, sizeof(question_t), questionCount, file);
    fclose(file);

    printf("Questions saved successfully to '%s'.\n", filename);
}

static void load_questions_from_file(const char *filename) {
    FILE *file = NULL;
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0 || !file) {
        printf("No existing quiz data found. Starting fresh.\n");
        return;
    }

    fread(&questionCount, sizeof(int), 1, file);
    fread(quiz, sizeof(question_t), questionCount, file);
    fclose(file);

    printf("Loaded %d question(s) from '%s'.\n", questionCount, filename);
}

// Gameplay Functions
static void display_question(const question_t *query, int index) {
    printf("\nQ%d: %s\n", index + 1, query->question);
    for (int i = 0; i < MAX_OPTIONS; ++i) {
        printf("   %d) %s\n", i + 1, query->options[i]);
    }
}

static int check_answer(const question_t *query, int user_choice) {
    return (user_choice == query->correct_option);
}

static void play_quiz(void) {
    int score = 0;
    int choice;

    for (int i = 0; i < questionCount; ++i) {
        display_question(&quiz[i], i);

        printf("Enter your answer (1-4): ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input. Moving to next question.\n");
            while (getchar() != '\n'); // flush buffer
            continue;
        }

        if (check_answer(&quiz[i], choice)) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer: %d) %s\n",
                   quiz[i].correct_option,
                   quiz[i].options[quiz[i].correct_option - 1]);
        }
    }

    show_score(score, questionCount);
}

static void show_score(int score, int total) {
    printf("\n---------------------------------\n");
    printf("Your final score: %d / %d\n", score, total);
    printf("---------------------------------\n");
}