/*
Mark-and-Sweep (C17)

Demonstrates a simple mark-and-sweep garbage collector in C by simulating
a tiny C-like "program". Each heap object can hold references to other
objects. The GC performs a mark phase from roots (simulated local variables)
and a sweep phase to free unreachable objects (including cycles).

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
#define MAX_OBJECTS 128
#define MAX_REFS_PER_OBJECT 8
#define MAX_ROOTS 64
#define LOG_FILE "marksweep.txt"

//  Struct Definitions
typedef struct ObjectStruct {
    int marked;                                         //  Mark bit for GC
    char *name;                                         //  Variable/obj name for logging
    char *value;                                        //   Payload as string
    struct ObjectStruct *refs[MAX_REFS_PER_OBJECT];     //   Contained references
    int ref_count;                                      //   Number of refs held
} object_struct_t;

//  Global Variables
static object_struct_t *objectPool[MAX_OBJECTS];
static int object_count = 0;

static object_struct_t *roots[MAX_OBJECTS];
static int root_count = 0;

static FILE *log_file = NULL;

//  Function Declarations
//  Logging
static void openLogFile(void);
static void closeLogFile(void);

//  Object Management
object_struct_t *allocObject(const char *name, const char *value);
static void addRef(object_struct_t *from, object_struct_t *to);
static void removeRef(object_struct_t *from, object_struct_t *to);
static void freeObject(object_struct_t *object);

//  Root Management
static void pushRoot(object_struct_t *object);
static void popRoot(void);

//  Mark & Sweep
static void mark(object_struct_t *object);
static void markAllRoots(void);
static void sweep(void);
static void gcCollect(void);

//  Utility Functions
static void cleanupAll(void);
static inline void free_s(void **ptr);

//  Simulation Functions
static void simulateTinyProgram(void);
static void simulateCycleEvent(void);

//  Utility Function: (Safe free)
static inline void free_s(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

//  Driver Code
int main(void) {
    printf("= Mark-and-Sweep GC Simulator =\n");

    openLogFile();

    simulateTinyProgram();
    simulateCycleEvent();

    /* Final force-cleanup if anything remains */
    fprintf(log_file, "\nFinal force-cleanup:\n");
    cleanupAll();

    closeLogFile();

    printf("Simulation complete. Log written to %s\n", LOG_FILE);
    return EXIT_SUCCESS;
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
object_struct_t *allocObject(const char *name, const char *value) {
    if (object_count >= MAX_OBJECTS) {
        fprintf(stderr, "ERROR: Object pool full\n");
        exit(EXIT_FAILURE);
    }

    object_struct_t *object = calloc(1, sizeof(object_struct_t));
    if (!object) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    object->marked = 0;
    object->name = _strdup(name);
    object->value = _strdup(value);
    object->ref_count = 0;

    objectPool[object_count++] = object;

    fprintf(log_file, "ALLOC: %s = %s\n", object->name, object->value);
    fflush(log_file);
    return object;
}

static void addRef(object_struct_t *from, object_struct_t *to) {
    if (!from || !to) return;
    if (from->ref_count >= MAX_REFS_PER_OBJECT) {
        fprintf(stderr, "ERROR: too many refs on %s\n", from->name);
        return;
    }

    from->refs[from->ref_count++] = to;
    fprintf(log_file, "ADD_REF: %s -> %s\n", from->name, to->name);
    fflush(log_file);
}

static void removeRef(object_struct_t *from, object_struct_t *to) {
    if (!from || !to) return;
    int i;
    for (i = 0; i < from->ref_count; i++) {
        if (from->refs[i] == to) break;
    }
    
    if (i == from->ref_count) return;
    for (; i + 1 < from->ref_count; i++) {
        from->refs[i] = from->refs[i + 1];
    }

    from->refs[--from->ref_count] = NULL;
    fprintf(log_file, "REMOVE_REF: %s -/-> %s\n", from->name, to->name);
    fflush(log_file);
}

static void freeObject(object_struct_t *object) {
    if (!object) return;
    fprintf(log_file, "FREE: %s (value=%s)\n", object->name, object->value);

    for (int i = 0; i < object->ref_count; i++) object->refs[i] = NULL;
    object->ref_count = 0;
    free_s((void **)&object->name);
    free_s((void **)&object->value);
    free_s((void **)&object);
}

//  Root management (simulate variables / scope)
static void pushRoot(object_struct_t *object) {
    if (root_count >= MAX_ROOTS) {
        fprintf(stderr, "ERROR: root stack overflow\n");
        exit(EXIT_FAILURE);
    }
    roots[root_count++] = object;
    fprintf(log_file, "PUSH_ROOT: %s\n", object ? object->name : "(NULL)");
    fflush(log_file);
}

static void popRoot(void) {
    if (root_count == 0) return;
    --root_count;
    fprintf(log_file, "POP_ROOT (now %d roots)\n", root_count);
    fflush(log_file);
}

//  Mark-and-Sweep Implementation
static void mark(object_struct_t *object) {
    if (!object) return;
    if (object->marked) return;
    object->marked = 1;
    fprintf(log_file, "MARK: %s\n", object->name);
    for (int i = 0; i < object->ref_count; ++i) {
        if (object->refs[i]) mark(object->refs[i]);
    }
}

static void markAllRoots(void) {
    fprintf(log_file, "= MARK PHASE START =\n");
    for (int i = 0; i < root_count; i++) {
        if (roots[i]) mark(roots[i]);
    }
    fprintf(log_file, "= MARK PHASE END =\n");
    fflush(log_file);
}

static void sweep(void) {
    fprintf(log_file, "= SWEEP PHASE START =\n");
    int distance = 0;
    for (int i = 0; i < object_count; i++) {
        object_struct_t *object = objectPool[i];
        if (!object) continue;
        if (object->marked) {
            object->marked = 0;
            objectPool[distance++] = 0;
        } else {
            freeObject(object);
        }
    }

    object_count = distance;
    for (int i = distance; i < MAX_OBJECTS; i++) objectPool[i] = NULL;
    fprintf(log_file, "= SWEEP PHASE END (object remaining: %d) =\n", object_count);
    fflush(log_file);
}

static void gcCollect(void) {
    fprintf(log_file, "\n- GC: Collecting-\n");
    markAllRoots();
    sweep();
    fprintf(log_file, "- GC: Done -\n\n");
    fflush(log_file);
}

//  Cleanup: Free any remaining objects
static void cleanupAll(void) {
    for (int i = 0; i < object_count; ++i) {
        if (objectPool[i]) {
            freeObject(objectPool[i]);
            objectPool[i] = NULL;
        }
    }
    object_count = 0;
}

//  Simulation of a Tiny "C Program"
static void simulateTinyProgram(void) {
    fprintf(log_file, "\n= Simulating tiny program =\n");
    fprintf(log_file, "int main() {\n");
    fprintf(log_file, "  int a = 5;\n");
    fprintf(log_file, "  if (a < 10) { a = a + 1; }\n");
    fprintf(log_file, "}\n\n");

    //  Simulate: int a = 5; -> allocate object and push as root
    object_struct_t *a = allocObject("a", "5");
    pushRoot(a);

    //  Evaluate condition: we might temporarily reference a (simulate)
    //  no extra root needed here; mark will find it from the root set

    //  Simulate 'a = a + 1' -> replace object 'a'
    //  Remove old value by replacing root with new object

    object_struct_t *a_new = allocObject("a", "6");

    popRoot();
    pushRoot(a_new);

    popRoot();

    gcCollect();
}

//  Simulate a scenario with a reference cycle:
static void simulateCycleEvent(void) {
    fprintf(log_file, "\n= Simulating cycle example =\n");
    object_struct_t *root_holder = allocObject("root_holder", "holder");
    pushRoot(root_holder);

    object_struct_t *x = allocObject("x", "obj_x");
    object_struct_t *y = allocObject("y", "obj_y");

    //  Make cycle: x -> y, y -> x
    addRef(x, y);
    addRef(y, x);

    //  Root_holder points to x and y (so they are reachable)
    addRef(root_holder, x);
    addRef(root_holder, y);

    //  Run GC: nothing should be collected
    gcCollect();

    //  Remove root_holder (simulate going out of scope)
    popRoot();

    //  Now x and y are reachable only from each other (cycle). mark phase should NOT mark them.
    gcCollect();

    removeRef(x, y);
    removeRef(y, x);    

    //  Clean any leftover objects
    gcCollect();    //  Safe to call again
}
