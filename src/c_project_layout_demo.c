/*
C Project Layout Demonstration

Description:
Educational example demonstrating a compiler-friendly C project 
structure with memory-safe operations. Manages a simple person 
database with basic CRUD operations and statistics calculations.

Standard: C17 (MSVC Safe Functions)

C Project Layout - Compiler Friendly Organization

1. // Short info of this program    -> Documentation Section
2. Includes                         -> Link/ File Inclusion Section
3. Constants & Macros               -> Define Constants, Macros & Compile-time Parameters
4. Struct/Enum Defs                 -> Structs & Enums Definitions Section
5. Global Variables                 -> Global Declaration Section
6. Function Declarations            -> Forward Declarations (use 'static' internally)
7. int main(void)                   -> main(void) Function Section
8. Function Definitions             -> Subprogram Section

Key Notes:
- Order matters! C is single-pass, so declare before use.
- Use 'static' keyword for file-scope visibility.
- This layout is ONE approach - not "the correct way". Many valid variations exist:
- Some devs skip function declarations entirely (define before use)
- Some place structs/enums AFTER constants/macros/globals
- Focus on logical flow and compiler requirements, not rigid rules

IMPORTANT:
This code uses C17 Standard with MSVC safe functions (_s suffix).
Works on Windows with MSVC/Clang-cl.

For Linux/macOS:
Replace fopen_s, strcpy_s, etc., with fopen, strcpy, if needed.
*/

// 1. Includes - Link/ File Inclusion Section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 2. Constants & Macros - Define Constants, Macros & Compile-time Parameters
#define MAX_PEOPLE 5
#define MIN_AGE 18
#define MAX_AGE 125

// Macro using global constant
#define INIT_PERSON(person) do { \
    strcpy_s((person).name, sizeof((person).name), DEFAULT_NAME); \
    (person).age = 0; \
    (person).salary = 0.0f; \
} while(0)

// 3. Struct/Enum Definitions - Structs & Enums Definitions Section
typedef enum Status {
    STATUS_SUCCESS,
    STATUS_ERROR,
    STATUS_NOT_FOUND
} status_t;

typedef struct Person {
    char name[50];
    int age;
    float salary;
} person_t;

// 4. Global Variables - Global Declaration Section
static int person_count = 0;  // File-scope only, prevents external linkage
const char *DEFAULT_NAME = "Unknown";

// 5. Function Declarations - Forward declarations (use 'static' internally)
static void initializePeople(person_t *people, size_t count);
static void printPerson(const person_t *person);
static void printAllPeople(const person_t *people, size_t count);
static status_t findPersonByName(const person_t *people, size_t count, const char *name, person_t *result);
static float calculateAverageSalary(const person_t *people, size_t count);
static void demonstratePersonManagement(void);

// Utility Function
// Safe free with NULL assignment to prevent dangling pointers
static inline void free_s(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

// Utility Function - clamp value between min and max
static inline int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// 6. int main(void) - main(void) Function Section
int main(void) {
    printf("= C Project Layout Demonstration =\n\n");
    
    demonstratePersonManagement();
    
    return EXIT_SUCCESS;
}

// 7. Function Definitions - Subprogram Section
static void initializePeople(person_t *people, size_t count) {
    const char *names[] = {"Alice", "Bob", "Charlie", "Diana", "Eve"};
    const int ages[] = {25, 32, 28, 45, 30};
    const float salaries[] = {50000.0f, 65000.0f, 55000.0f, 80000.0f, 60000.0f};

    for (size_t i = 0; i < count && i < MAX_PEOPLE; i++) {
        strcpy_s(people[i].name, sizeof(people[i].name), names[i]);
        people[i].age = clamp(ages[i], MIN_AGE, MAX_AGE);
        people[i].salary = salaries[i];
        person_count++;
    }
}

static void printPerson(const person_t *person) {
    if (!person) {
        printf("Error: NULL person pointer\n");
        return;
    }
    printf("Name: %-15s\tAge: %3d\tSalary: $%.2f\n",
    person->name, person->age, person->salary);
}

static void printAllPeople(const person_t *people, size_t count) {
    if (count == 0) {
        printf("No people to display.\n");
        return;
    }
    
    printf("\n= People Directory (%zu people) =\n", count);
    for (size_t i = 0; i < count; i++) {
        printf("[%zu] ", i + 1);
        printPerson(&people[i]);
    }
    printf("\n");
}

static status_t findPersonByName(const person_t *people, size_t count, 
                                  const char *name, person_t *result) {
    if (!people || !name || !result) {
        return STATUS_ERROR;
    }

    for (size_t i = 0; i < count; i++) {
        if (strcmp(people[i].name, name) == 0) {
            *result = people[i];
            return STATUS_SUCCESS;
        }
    }
    
    return STATUS_NOT_FOUND;
}

static float calculateAverageSalary(const person_t *people, size_t count) {
    if (count == 0) return 0.0f;

    float total = 0.0f;
    for (size_t i = 0; i < count; i++) {
        total += people[i].salary;
    }
    
    return total / count;
}

static void demonstratePersonManagement(void) {
    // Allocate memory for people
    person_t *people = calloc(MAX_PEOPLE, sizeof(person_t));
    if (!people) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Initialize data
    initializePeople(people, MAX_PEOPLE);
    
    // Display all people
    printAllPeople(people, MAX_PEOPLE);
    
    // Find specific person
    person_t found;
    INIT_PERSON(found);  // Using macro defined in section 4
    
    const char *searchName = "Charlie";
    status_t status = findPersonByName(people, MAX_PEOPLE, searchName, &found);
    
    if (status == STATUS_SUCCESS) {
        printf("Found person '%s':\n", searchName);
        printPerson(&found);
    } else if (status == STATUS_NOT_FOUND) {
        printf("Person '%s' not found\n", searchName);
    } else {
        printf("Error searching for person\n");
    }
    
    // Calculate statistics
    float avgSalary = calculateAverageSalary(people, MAX_PEOPLE);
    printf("\nAverage Salary: $%.2f\n", avgSalary);
    
    // Show global state
    printf("Total people processed: %d\n", person_count);
    
    // Safe cleanup using custom free_s utility
    free_s((void **)&people);
    
    printf("\nMemory safely freed (pointer set to NULL)\n");
}