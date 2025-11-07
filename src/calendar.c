/*
Simple Calendar Application in C17

Features:
- Find the day of a given date
- Print all days of a given month

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

//  Function Declarations
static void clearInputBuffer(void);
static int isLeapYear(int year);
static int getDaysInMonth(int month, int year);
static int dayOfWeek(int day, int month, int year);
static void findDay(void);
static void printMonthCalendar(void);

//  Utility Function
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//  Utility Function - Leap Year
static int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//  Driver Code
int main(void) {
    int choice;

    while (1) {
        printf("\n= CALENDAR MENU =\n");
        printf("1. Find Day of a Date\n");
        printf("2. Print Month Calendar\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf_s("%d", &choice) != 1) {
            while (getchar() != '\n'); // clear invalid input
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                findDay();
                break;
            case 2:
                printMonthCalendar();
                break;
            case 3:
                printf("Exiting Calendar Program.\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}

//  Get number of days in a month
static int getDaysInMonth(int month, int year) {
    switch (month) {
        case 1: return 31;
        case 2: return isLeapYear(year) ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }
}

//  Get day of the week using Zeller’s Congruence
static int dayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int yearOfCentury = year % 100;    // Last two digits of the year
    int centuryPart   = year / 100;    // Century (e.g., 20 for 2024)
    int zellerValue = (day + (13 * (month + 1)) / 5 + yearOfCentury
                       + (yearOfCentury / 4) + (centuryPart / 4)
                       + (5 * centuryPart)) % 7;
    
    // Adjust Zeller’s output to make Sunday = 0, Monday = 1, ...
    int dayIndex = (zellerValue + 6) % 7;
    return dayIndex;
}

static void findDay(void) {
    int day, month, year;
    const char *dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                              "Thursday", "Friday", "Saturday"};

    printf("\n- Find Day of a Given Date -\n");
    printf("Enter day (1-31): ");
    scanf_s("%d", &day);
    printf("Enter month (1-12): ");
    scanf_s("%d", &month);
    printf("Enter year: ");
    scanf_s("%d", &year);

    if (month < 1 || month > 12 || day < 1 || day > getDaysInMonth(month, year)) {
        printf("Invalid date entered.\n");
        return;
    }

    int dayIndex = dayOfWeek(day, month, year);
    printf("Date: %02d-%02d-%d is a %s.\n", day, month, year, dayNames[dayIndex]);
}

// Print all days of a month
static void printMonthCalendar(void) {
    int month, year;
    printf("\n= Print Month Calendar =\n");
    printf("Enter month (1-12): ");
    scanf_s("%d", &month);
    printf("Enter year: ");
    scanf_s("%d", &year);

    int days = getDaysInMonth(month, year);
    int startDay = dayOfWeek(1, month, year);

    const char *monthNames[] = {"January", "February", "March", "April", "May", "June",
                                "July", "August", "September", "October", "November", "December"};
    printf("\n     %s %d\n", monthNames[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");

    // Print initial spaces for start day
    for (int i = 0; i < startDay; i++) {
        printf("   ");
    }

    // Print all days
    for (int d = 1; d <= days; d++) {
        printf("%2d ", d);
        if ((startDay + d) % 7 == 0)
            printf("\n");
    }
    printf("\n");
}