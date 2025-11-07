/*
Syntax Checker - Final Version
Validates that parentheses (), braces {}, and brackets [] are properly balanced
in a given C source file. Reports syntax issues with line numbers and summary.

Build:
    clang -std=c17 -Wall -Wextra -Werror -g -O0 syntax_checker_final.c -o syntax_checker.exe

Usage:
    syntax_checker.exe <source_file.c>

Example:
    syntax_checker.exe sample_syntax1.c

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
#include <stdlib.h>

//  Constants & Macros
#define MAX_STACK_SIZE 256

//  Struct Definitions
typedef struct Stack {
    char data[MAX_STACK_SIZE];
    int top;
} stack_t;

//  Function Declarations
static void stackInit(stack_t *stack);
static int  stackIsEmpty(const stack_t *stack);
static int  stackIsFull(const stack_t *stack);
static void stackPush(stack_t *stack, char symbol);
static char stackPop(stack_t *stack);
static int  isMatchingPair(char opening, char closing);
static void checkSyntaxBalance(const char *filename);

//  Driver Code
int main(int argc, char *argv[]) {
    printf("SYNTAX CHECKER - FINAL VERSION\n");

    if (argc < 2) {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return EXIT_FAILURE;
    }

    checkSyntaxBalance(argv[1]);
    return 0;
}

//  Function Definitions
static void stackInit(stack_t *stack) {
    stack->top = -1;
}

static int stackIsEmpty(const stack_t *stack) {
    return stack->top == -1;
}

static int stackIsFull(const stack_t *stack) {
    return stack->top >= MAX_STACK_SIZE - 1;
}

static void stackPush(stack_t *stack, char symbol) {
    if (stackIsFull(stack)) {
        fprintf(stderr, "ERROR: Stack overflow while pushing '%c'\n", symbol);
        exit(EXIT_FAILURE);
    }
    stack->data[++(stack->top)] = symbol;
}

static char stackPop(stack_t *stack) {
    if (stackIsEmpty(stack)) {
        return '\0';
    }
    return stack->data[(stack->top)--];
}

//  Matching Pair Logic
static int isMatchingPair(char opening, char closing) {
    return  (opening == '(' && closing == ')') ||
            (opening == '{' && closing == '}') ||
            (opening == '[' && closing == ']');
}

//  Syntax Checking Logic
static void checkSyntaxBalance(const char *filename) {
    FILE *source_file = NULL;
    errno_t error = fopen_s(&source_file, filename, "r");

    if (error != 0 || !source_file) {
        fprintf(stderr, "ERROR: Unable to open file: %s\n", filename);
        return;
    }

    printf("\n---------------------------------\n");
    printf("= SYNTAX CHECK REPORT = \n");
    printf("\n---------------------------------\n");
    printf("File: %s\n\n", filename);

    stack_t stack;
    stackInit(&stack);

    int line_number = 1;
    int error_count = 0;
    char symbol;

    while ((symbol = fgetc(source_file)) != EOF) {
        if (symbol == '\n')
            ++line_number;

        if (symbol == '(' || symbol == '{' || symbol == '[') {
            stackPush(&stack, symbol);
        } 
        else if (symbol == ')' || symbol == '}' || symbol == ']') {
            char top = stackPop(&stack);

            if (top == '\0') {
                printf("Line %-4d | Extra closing '%c'\n", line_number, symbol);
                ++error_count;
            } 
            else if (!isMatchingPair(top, symbol)) {
                printf("Line %-4d | Mismatch '%c' (opened with '%c')\n",
                       line_number, symbol, top);
                ++error_count;
            }
        }
    }

    fclose(source_file);

    //  Check for remaining unclosed symbols
    while (!stackIsEmpty(&stack)) {
        printf("End of file | Missing closing for '%c'\n", stackPop(&stack));
        ++error_count;
    }

    printf("\n---------------------------------\n");
    if (error_count == 0)
        printf("RESULT: Syntax is properly balanced \n");
    else
        printf("RESULT: Found %d syntax issue(s) \n", error_count);
    printf("\n---------------------------------\n");
    }