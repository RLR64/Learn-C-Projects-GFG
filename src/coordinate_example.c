/*
Coordinate Distance Calculator in C17

Features:
- Read two points from user input
- Calculate distance using Euclidean distance formula
- Display points and distance result
- Struct-based point representation
- Math library usage (sqrt)
- Inline functions for performance

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
#include <math.h>

//  Constants & Macros
#define MAX_INPUT_LENGTH 100

//  Struct Definitions
typedef struct Point {
    int x;
    int y;
} point_t;

//  Function Declarations
static point_t readPoint(const char *prompt);
static void printPoint(const char *label, point_t point);
static inline float calculateDistance(point_t a, point_t b);

//  Driver Code
int main(void) {
    point_t point1 = readPoint("Enter coordinates for Point 1");
    point_t point2 = readPoint("Enter coordinates for Point 2");

    //  Display points
    printPoint("Point 1", point1);
    printPoint("Point 2", point2);
    
    // Step 3: Compute and display distance
    printf("Distance: %.2f units\n", calculateDistance(point1, point2));
    return 0;
}

//  Function Definitions
static point_t readPoint(const char *prompt) {
    point_t point;
    printf("%s (format: x y): ", prompt);
    scanf_s("%d %d", &point.x, &point.y);
    return point;
}

static void printPoint(const char *label, point_t point) {
    printf("%s: (%d, %d)\n", label, point.x, point.y);
}

static inline float calculateDistance(point_t a, point_t b) {
    int distanceX = b.x - a.x;
    int distanceY = b.y - a.y;
    return sqrtf((float)(distanceX * distanceX + distanceY * distanceY));
}