/*
Common C Keywords for Beginners

| Category             | Keywords                                                       | Notes                                  |
| -------------------- | -------------------------------------------------------------- | -------------------------------------- |
| **Data types**       | `int`, `float`, `double`, `char`, `_Bool`, `void`              | Basic types; `_Bool` is C99+           |
| **Control flow**     | `if`, `else`, `switch`, `case`, `default`, `break`, `continue` | Standard branching and loop controls   |
| **Loops**            | `for`, `while`, `do`                                           | Iteration constructs                   |
| **Storage class**    | `static`, `extern`, `register`, `auto`                         | Controls variable lifetime and linkage |
| **Function-related** | `return`                                                       | Returns a value from a function        |
| **Modifiers**        | `const`, `signed`, `unsigned`, `short`, `long`                 | Modify base types                      |
| **Other**            | `sizeof`, `struct`, `typedef`, `enum`                          | Structs, aliases, and enumerations     |
| **Boolean / null**   | `true`, `false`, `NULL`                                        | `_Bool` usage                          |

Common C Standard Library Functions for Beginners

| Header       | Functions                                                              | Purpose                                         |
| ------------ | ---------------------------------------------------------------------- | ----------------------------------------------- |
| `<stdio.h>`  | `printf`, `scanf`, `puts`, `gets` (unsafe), `fgets`                    | Input/output                                    |
| `<stdlib.h>` | `malloc`, `calloc`, `free`, `rand`, `srand`, `qsort`, `abs`            | Memory, random numbers, sorting, absolute value |
| `<string.h>` | `strcpy`, `strncpy`, `strcat`, `strlen`, `strcmp`, `_strdup` (Windows) | String manipulation                             |
| `<math.h>`   | `sqrt`, `pow`, `sin`, `cos`, `tan`                                     | Basic math operations                           |
| `<time.h>`   | `time`, `clock`                                                        | Get time, seed RNG                              |

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
#include <string.h>
#include <math.h>
#include <time.h>

//  Constant Definition
const int INITIAL_STUDENTS = 10;

//  Struct and Enum Definitions
typedef struct Student {
    char name[50];
    char subject[20];
    float gpa;
} Student;

typedef enum Subject {
    MATH,
    PHYSICS,
    CHEMISTRY,
    COMPUTER_SCIENCE,
    STEM_COUNT
} Subject;

typedef struct SubjectGroup {
    Student *students;  // Dynamic array for this subject
    size_t count;       // Number of students currently stored
    size_t capacity;    // Allocated size
} SubjectGroup;

//  Function Declarations - Example of separating the Function Declarations into logical order

// Memory Management
static Student *initializeStudents(void);
static void cleanupGroups(SubjectGroup *groups, size_t groupCount);

// Data Generation
static float randomGPA(void);
static void generateRandomStudents(Student *students, size_t count);

// Sorting & Comparison
static int compareByName(const void *firstStudent, const void *secondStudent);
static int compareByGPA(const void *firstStudent, const void *secondStudent);
static void sortByName(Student *students, size_t count);
static void sortByGPA(Student *students, size_t count);

// Display & Utilities
static const char *getSubjectName(Subject subject);
static void printStudents(const Student *students, size_t count);

// Demonstrations (High-level operations)
static void demonstrateSorting(Student *students, size_t count);
static void demonstrateGrouping(const Student *students, size_t count);

//  Helper Function
static float randomGPA(void) {
    return ((float)rand() / RAND_MAX) * 4.0f;
}

//  Driver Code
int main(void) {
    srand((unsigned)time(NULL));
    
    Student *students = initializeStudents();
    if (!students) {
        return EXIT_FAILURE;
    }
    
    demonstrateSorting(students, INITIAL_STUDENTS);
    demonstrateGrouping(students, INITIAL_STUDENTS);
    
    free(students);
    students = NULL;

    return EXIT_SUCCESS;
}

//  Function Definitions
//  Memory Management
static Student *initializeStudents(void) {
    Student *students = calloc(INITIAL_STUDENTS, sizeof(Student));
    if (!students) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    generateRandomStudents(students, INITIAL_STUDENTS);
    
    printf("Original Student List:");
    printStudents(students, INITIAL_STUDENTS);
    
    return students;
}

static void cleanupGroups(SubjectGroup *groups, size_t groupCount) {
    for (size_t i = 0; i < groupCount; i++) {
        free(groups[i].students);
        groups[i].students = NULL;
    }
}

static void generateRandomStudents(Student *students, size_t count) {
    const char *names[] = {
        "Alice", "Bob", "Charlie", "Diana", "Evan",
        "Fiona", "George", "Hannah", "Ian", "Julia"
    };

    for (size_t i = 0; i < count; i++) {
        strcpy_s(students[i].name, sizeof(students[i].name), names[i]);
        Subject subject = rand() % STEM_COUNT;
        strcpy_s(students[i].subject, sizeof(students[i].subject), getSubjectName(subject));
        students[i].gpa = randomGPA();
    }
}

// Sorting & Comparison
static int compareByName(const void *firstStudent, const void *secondStudent) {
    const Student *studentA = (const Student *)firstStudent;
    const Student *studentB = (const Student *)secondStudent;
    return strcmp(studentA->name, studentB->name);
}

static int compareByGPA(const void *firstStudent, const void *secondStudent) {
    const Student *studentA = (const Student *)firstStudent;
    const Student *studentB = (const Student *)secondStudent;
    if (studentA->gpa < studentB->gpa) return 1;
    else if (studentA->gpa > studentB->gpa) return -1;
    return 0;
}

static void sortByName(Student *students, size_t count) {
    qsort(students, count, sizeof(Student), compareByName);
}

static void sortByGPA(Student *students, size_t count) {
    qsort(students, count, sizeof(Student), compareByGPA);
}

// Display & Utilities
static const char *getSubjectName(Subject subject) {
    switch (subject) {
        case MATH: return "Math";
        case PHYSICS: return "Physics";
        case CHEMISTRY: return "Chemistry";
        case COMPUTER_SCIENCE: return "Computer Science";
        default: return "Unknown";
    }
}

static void printStudents(const Student *students, size_t count) {
    if (count == 0) {
        printf("No students to display.\n");
        return;
    }
    printf("\n%-15s %-20s %-5s\n", "Name", "Subject", "GPA");
    printf("-------------------------------------------\n");
    for (size_t i = 0; i < count; i++) {
        printf("%-15s %-20s %.2f\n", students[i].name, students[i].subject, students[i].gpa);
    }
    printf("\n");
}

// Demonstrations (High-level operations)
static void demonstrateSorting(Student *students, size_t count) {
    sortByName(students, count);
    printf("Students Sorted by Name:");
    printStudents(students, count);

    sortByGPA(students, count);
    printf("Students Sorted by GPA:");
    printStudents(students, count);
}

static void demonstrateGrouping(const Student *students, size_t count) {
    // Initialize groups
    SubjectGroup groups[STEM_COUNT];
    for (int i = 0; i < STEM_COUNT; i++) {
        groups[i].students = calloc(1, sizeof(Student));
        if (!groups[i].students) {
            fprintf(stderr, "Memory allocation failed for group %d.\n", i);
            // Cleanup previously allocated groups
            for (int j = 0; j < i; j++) {
                free(groups[j].students);
            }
            return;
        }
        groups[i].count = 0;
        groups[i].capacity = 1;
    }

    // Assign students to groups
    for (size_t i = 0; i < count; i++) {
        Subject subject;
        if (strcmp(students[i].subject, "Math") == 0) subject = MATH;
        else if (strcmp(students[i].subject, "Physics") == 0) subject = PHYSICS;
        else if (strcmp(students[i].subject, "Chemistry") == 0) subject = CHEMISTRY;
        else subject = COMPUTER_SCIENCE;

        SubjectGroup *group = &groups[subject];

        // Resize if needed
        if (group->count >= group->capacity) {
            group->capacity *= 2;
            Student *temp = realloc(group->students, group->capacity * sizeof(Student));
            if (!temp) {
                fprintf(stderr, "Realloc failed for subject group.\n");
                cleanupGroups(groups, STEM_COUNT);
                return;
            }
            group->students = temp;
        }
        
        // Add student to group
        group->students[group->count++] = students[i];
    }

    // Print students grouped by subject
    for (int i = 0; i < STEM_COUNT; i++) {
        if (groups[i].count == 0) continue;
        printf("Students in %s:\n", getSubjectName((Subject)i));
        printStudents(groups[i].students, groups[i].count);
    }

    // Cleanup
    cleanupGroups(groups, STEM_COUNT);
}