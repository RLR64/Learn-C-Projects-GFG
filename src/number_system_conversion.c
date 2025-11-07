/*
Number System Conversion (C17)

Features:
- Decimal -> Binary
- Decimal -> Octal
- Hexadecimal -> Binary
- Binary -> Hexadecimal
- Input validation and safe memory handling

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
#include <ctype.h>

//  Function Declarations
static void decimalToBinary(void);
static void binaryToDecimal(void);
static void decimalToOctal(void);
static void octalToDecimal(void);
static void hexadecimalToBinary(void);
static void binaryToHexadecimal(void);
static void clearInputBuffer(void);
static void displayMenu(void);

//  Utility Function
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//  Driver Code
int main(void) {
    int choice;
    do {
        displayMenu();
        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n\n");
            continue;
        }

        switch (choice) {
            case 1: decimalToBinary(); break;
            case 2: binaryToDecimal(); break;
            case 3: decimalToOctal(); break;
            case 4: octalToDecimal(); break;
            case 5: hexadecimalToBinary(); break;
            case 6: binaryToHexadecimal(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n\n");
        }
    } while (choice != 0);

    return 0;
}

//  Function Definitions
static void decimalToBinary(void) {
    int number;
    int binary[64];  // 64-bit integer for safety
    int i = 0;

    printf("Enter a positive decimal number: ");
    if (scanf_s("%d", &number) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (number < 0) {
        printf("Please enter a positive number.\n");
        return;
    }

    if (number == 0) {
        printf("Binary: 0\n\n");
        return;
    }

    while (number > 0) {
        binary[i++] = number % 2;
        number /= 2;
    }

    printf("Binary: ");
    for (int j = i - 1; j >= 0; j--)
        printf("%d", binary[j]);
    printf("\n\n");
}

static void binaryToDecimal(void) {
    char *binary = (char *)calloc(65, sizeof(char)); // 64 bits max
    if (!binary) { printf("Memory allocation failed.\n"); return; }

    printf("Enter a binary number (up to 64 bits): ");
    if (scanf_s("%64s", binary) != 1) { clearInputBuffer(); free(binary); return; }

    long decimal = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            printf("Invalid binary digit: %c\n\n", binary[i]);
            free(binary);
            return;
        }
        decimal = decimal * 2 + (binary[i] - '0');
    }

    printf("Decimal: %ld\n\n", decimal);
    free(binary);
    binary = NULL;
}

static void decimalToOctal(void) {
    int number;
    printf("Enter a positive decimal number: ");
    if (scanf_s("%d", &number) != 1) { clearInputBuffer(); return; }

    if (number < 0) {
        printf("Please enter a positive number.\n\n");
        return;
    }

    printf("Octal: %o\n\n", number);
}

static void octalToDecimal(void) {
    char *octal = (char *)calloc(32, sizeof(char)); // Up to 31 digits
    if (!octal) { printf("Memory allocation failed.\n"); return; }

    printf("Enter an octal number: ");
    if (scanf_s("%31s", octal) != 1) { free(octal); return; }

    long decimal = 0;
    for (int i = 0; octal[i] != '\0'; i++) {
        if (octal[i] < '0' || octal[i] > '7') {
            printf("Invalid octal digit: %c\n\n", octal[i]);
            free(octal);
            return;
        }
        decimal = decimal * 8 + (octal[i] - '0');
    }

    printf("Decimal: %ld\n\n", decimal);
    free(octal);
    octal = NULL;
}

static void hexadecimalToBinary(void) {
    char *hexadecimal = (char *)calloc(32, sizeof(char));
    if (!hexadecimal) { printf("Memory allocation failed.\n"); return; }

    printf("Enter a hexadecimal number: ");
    if (scanf_s("%31s", hexadecimal) != 1) { free(hexadecimal); return; }

    printf("Binary: ");
    for (int i = 0; hexadecimal[i] != '\0'; i++) {
        char c = toupper(hexadecimal[i]);
        switch (c) {
            case '0': printf("0000"); break;
            case '1': printf("0001"); break;
            case '2': printf("0010"); break;
            case '3': printf("0011"); break;
            case '4': printf("0100"); break;
            case '5': printf("0101"); break;
            case '6': printf("0110"); break;
            case '7': printf("0111"); break;
            case '8': printf("1000"); break;
            case '9': printf("1001"); break;
            case 'A': printf("1010"); break;
            case 'B': printf("1011"); break;
            case 'C': printf("1100"); break;
            case 'D': printf("1101"); break;
            case 'E': printf("1110"); break;
            case 'F': printf("1111"); break;
            default:
                printf("\nInvalid hex digit: %c\n\n", c);
                free(hexadecimal);
                return;
        }
    }
    printf("\n\n");
    free(hexadecimal);
    hexadecimal = NULL;
}

static void binaryToHexadecimal(void) {
    char *binary = (char *)calloc(65, sizeof(char));
    if (!binary) { printf("Memory allaction failed.\n"); return; }

    printf("Enter a binary number (up to 64 bits): ");
    if (scanf_s("%64s", binary) != 1) { free(binary); return; }

    //  Validate Input
    for (int i = 0; binary[i] != '\0'; i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            printf("Invalid binary digit: %c\n\n", binary[i]);
            free(binary);
            return;
        }
    }

    // Convert to decimal, then to hex
    long decimal = strtol(binary, NULL, 2);
    printf("Hexadecimal: %lX\n\n", decimal);
    free(binary);
    binary = NULL;
}

//  Display Menu Options
static void displayMenu(void) {
    printf("= Number System Conversion =\n");
    printf("1. Decimal to Binary\n");
    printf("2. Binary to Decimal\n");
    printf("3. Decimal to Octal\n");
    printf("4. Octal to Decimal\n");
    printf("5. Hexadecimal to Binary\n");
    printf("6. Binary to Hexadecimal\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}