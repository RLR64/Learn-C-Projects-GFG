/*
Multidimensional Arrays Example (C17)

Features:
- Initialization of a 2D array
- Iteration through elements
- Summation via helper function
- Clean modular structure

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

//  Constant & Macros
#define ROWS 3
#define COLUMNS 4

//  Function Declarations
static int sumGrid(int grid[ROWS][COLUMNS]);
static void displayGrid(int grid[ROWS][COLUMNS]);
static void displayMain(int grid[ROWS][COLUMNS]);

//  Driver Code
int main(void) {
    int grid[ROWS][COLUMNS] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    displayMain(grid);
    return 0;
}

//  Function Declarations
//  Sum all elements in the grid/matrix
static int sumGrid(int grid[ROWS][COLUMNS]) {
    int total = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
            total += grid[i][j];
    return total;
}

//  Display grid with neat formatting
static void displayGrid(int grid[ROWS][COLUMNS]) {
    printf("\n2D Grid:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++)
            printf("%3d ", grid[i][j]);
        printf("\n");
    }
}

//  Display grid and computed statistics
static void displayMain(int grid[ROWS][COLUMNS]) {
    displayGrid(grid);
    int total = sumGrid(grid);
    double average = (double)total / (ROWS * COLUMNS);
    printf("\nTotal sum of elements: %d\n", total);
    printf("Average value: %.2f\n", average);
}