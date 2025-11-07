/*
Simple Calculator (C17)

Features:
- const static for read-only constants
- static for internal helper functions (file scope)

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

// Read-only instruction text (file local)
const static char MENU_TEXT[] =
    "= Simple Calculator =\n"
    "Operations: +, -, *, /\n"
    "Enter 'q' to quit.\n\n";

//  Function Declarations
static float performOperation(char chosen_operator, float a, float b);
static void clearInputBuffer(void);
static float performOperation(char chosen_operator, float a, float b) {
    switch (chosen_operator) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0.0f) {
                printf("Error: Division by zero!\n");
                return 0.0f;
            }
            return a / b;
        default:
            printf("Invalid operation.\n");
            return 0.0f;
    }
}

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ; // discard
    }
}

// Driver Code
int main(void) {
    char operation_selected;
    float number1, number2, result;

    printf("%s", MENU_TEXT);

    while (1) {
        printf("Enter operations (+, -, *, /) or q to quit: ");
        if (scanf_s(" %c", &operation_selected) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (operation_selected == 'q' || operation_selected == 'Q') {
            printf("Goodbye!\n");
            break;
        }

        printf("Enter first number: ");
        if (scanf_s("%f", &number1) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }
        printf("Enter second number: ");
        if (scanf_s("%f", &number2) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        result = performOperation(operation_selected, number1, number2);
        printf("Result: %.2f\n\n", result);

    }
    return 0;
}