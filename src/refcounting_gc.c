/*
Reference Counting (C17)

Demonstrates a basic reference counting garbage collector in C by simulating
a tiny C-like "program" with variable creation, assignment, and block scope.
Each variable is represented by a dynamically allocated object with a
reference count. References are incremented/decremented as simulated code
executes, and memory is freed automatically when refcount reaches zero.

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

//  Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Constants & Macros
#define MAX_OBJECTS 100
#define LOG_FILE "refcount.txt"

//  Struct Definitions
typedef struct ObjectStruct {
    int refcount;
    char *name;
    char *value;
} object_t;

//  Global Variables
static object_t *objectPool[MAX_OBJECTS];
static int object_count = 0;
static FILE *log_file = NULL;   // global log file pointer

//  Function Declarations
static void openLogFile(void);
static void closeLogFile(void);
object_t *createObject(const char *name, const char *value);
static void retainObject(object_t *obj);
static void releaseObject(object_t *obj);
static void cleanup(void);
static inline void free_s(void **ptr);
static void simulateProgram(void);

//  Utility Function: (Safe free)
static inline void free_s(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

//  Logging Functions
static void openLogFile(void) {
    errno_t error = fopen_s(&log_file, LOG_FILE, "w");
    if (error != 0 || log_file == NULL) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", LOG_FILE);
        exit(EXIT_FAILURE);
    }
}

static void closeLogFile(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

//  Object Management Functions
object_t *createObject(const char *name, const char *value) {
    if (object_count >= MAX_OBJECTS) {
        fprintf(stderr, "ERROR: Object pool full.\n");
        exit(EXIT_FAILURE);
    }

    object_t *object = calloc(1, sizeof(object_t));
    if (!object) {
        fprintf(stderr, "ERROR: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    object->refcount = 1;
    object->name = _strdup(name);
    object->value = _strdup(value);
    objectPool[object_count++] = object;

    fprintf(log_file, "Created %s = %s (refs = %d)\n", object->name, object->value, object->refcount);
    fflush(log_file);
    return object;
}

static void retainObject(object_t *object) {
    if (object) {
        object->refcount++;
        fprintf(log_file, "Retained %s (refs = %d)\n", object->name, object->refcount);
        fflush(log_file);
    }
}

static void releaseObject(object_t *object) {
    if (!object) return;

    object->refcount--;
    fprintf(log_file, "Released %s (refs = %d)\n", object->name, object->refcount);

    if (object->refcount <= 0)  {
        fprintf(log_file, "Freeing %s\n", object->name);
        free_s((void **)&object->name);
        free_s((void **)&object->value);
        free_s((void **)&object);
    }

    fflush(log_file);
}

static void cleanup(void) {
    for (int i = 0; i < object_count; i++) {
        if (objectPool[i]) {
            fprintf(log_file, "Force cleanup: %s\n", objectPool[i]->name);
            free_s((void **)&objectPool[i]->name);
            free_s((void **)&objectPool[i]->value);
            free_s((void **)&objectPool[i]);
            objectPool[i] = NULL;
        }
    }
    object_count = 0;
    fflush(log_file);
}

//  Driver Code
int main(void) {
    printf("= Reference Counting Simulator =\n");

    openLogFile();
    simulateProgram();
    cleanup();
    closeLogFile();

    printf("Simulation complete. Log written to %s\n", LOG_FILE);
    return EXIT_SUCCESS;
}

//  Simulation of a Tiny "C Program"
static void simulateProgram(void) {
    fprintf(log_file, "= Simulating Tiny C Program =\n\n");
    fprintf(log_file, "int main() {\n");
    fprintf(log_file, "    int a = 5;\n");
    fprintf(log_file, "    if (a < 10) {\n");
    fprintf(log_file, "        a = a + 1;\n");
    fprintf(log_file, "    }\n");
    fprintf(log_file, "    return 0;\n");
    fprintf(log_file, "}\n\n");

    // Simulate: int a = 5;
    object_t *a = createObject("a", "5");

    // Simulate: if (a < 10)
    retainObject(a); // temporary use for condition

    // Simulate block scope:
    if (atoi(a->value) < 10) {
        // Simulate: a = a + 1
        releaseObject(a); // release old value
        a = createObject("a", "6"); // new assignment
    }

    //  End of block
    releaseObject(a);
    fprintf(log_file, "\n= Program End =\n");
    fflush(log_file);
}
