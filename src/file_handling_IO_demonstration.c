/*
Program: File Handling and I/O Demonstration (C17)

Description:
Educational example demonstrating major file I/O types and functions from <stdio.h>.
Includes examples of file access, formatted/unformatted input/output, positioning,
and error handling in a safe, MSVC-compatible way.

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

// Constants & Macros
#define SAMPLE_FILE "sample.txt"
#define COPY_FILE   "copy.txt"
#define BUFFER_SIZE 128

// Global Variables
static FILE *fp = NULL;     // demonstration file handle
static FILE *fp_copy = NULL;

// ---------------------------------------------------------------------------
// Function Declarations
// ---------------------------------------------------------------------------
static void demonstrateFileAccess(void);
static void demonstrateUnformattedIO(void);
static void demonstrateFormattedIO(void);
static void demonstratePositioning(void);
static void demonstrateErrorHandling(void);
static void demonstrateFileOperations(void);
static void safeClose(FILE **file);
static void pauseConsole(void);

// System Portability Check:
static void checkSystemPortability(void) {
    printf("= System Portability Check =\n");

#if defined(_WIN32) || defined(_WIN64)
    printf("Operating System : Windows\n");
    printf("Note: fopen_s and printf_s are available (MSVC Safe Functions)\n");
#elif defined(__linux__)
    printf("Operating System : Linux\n");
    printf("Note: fopen_s not available — fallback to fopen instead.\n");
#elif defined(__APPLE__) && defined(__MACH__)
    printf("Operating System : macOS\n");
    printf("Note: fopen_s not available — using standard fopen.\n");
#elif defined(__unix__)
    printf("Operating System : Generic UNIX\n");
#elif defined(__posix__)
    printf("Operating System : POSIX-compliant system\n");
#else
    printf("Operating System : Unknown / Unsupported Platform\n");
#endif

    printf("Compiler: ");
#if defined(_MSC_VER)
    printf("MSVC (Microsoft Visual C++)\n");
#elif defined(__clang__)
    printf("Clang\n");
#elif defined(__GNUC__)
    printf("GCC\n");
#else
    printf("Unknown compiler\n");
#endif
    printf("\n");
}

// ---------------------------------------------------------------------------
// Utility - Safe fclose
// ---------------------------------------------------------------------------
static void safeClose(FILE **file) {
    if (file && *file) {
        fclose(*file);
        *file = NULL;
    }
}

// Pause (for visual console output)
static void pauseConsole(void) {
    printf("\nPress ENTER to continue...");
    getchar();
}

// ---------------------------------------------------------------------------
// Driver Code (main)
// ---------------------------------------------------------------------------
int main(void) {
    printf("= File Handling & I/O Demonstration =\n\n");
    checkSystemPortability();
    demonstrateFileAccess();
    demonstrateUnformattedIO();
    demonstrateFormattedIO();
    demonstratePositioning();
    demonstrateErrorHandling();
    demonstrateFileOperations();

    printf("\nAll demonstrations completed.\n");
    return EXIT_SUCCESS;
}

// ---------------------------------------------------------------------------
// Function Definitions
// ---------------------------------------------------------------------------

// 1. File Access Functions
static void demonstrateFileAccess(void) {
    printf("1. File Access Demonstration\n");

    errno_t err = fopen_s(&fp, SAMPLE_FILE, "w+");
    if (err != 0 || !fp) {
        perror("fopen_s failed");
        exit(EXIT_FAILURE);
    }
    printf("File '%s' created successfully.\n", SAMPLE_FILE);

    // Write sample text
    fputs("Hello, file handling in C!\n", fp);
    fputs("This is a second line.\n", fp);

    fflush(fp); // flush to disk
    rewind(fp); // reset pointer for reading

    printf("File written and rewound.\n\n");
    pauseConsole();
}

// 2. Unformatted Input/Output
static void demonstrateUnformattedIO(void) {
    printf("2. Unformatted I/O Demonstration\n");

    char buffer[BUFFER_SIZE];
    printf("Reading file line-by-line using fgets_s():\n");

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("Line read: %s", buffer);
    }

    // Demonstrate fgetc / ungetc
    rewind(fp);
    printf("\nReading first char with fgetc(): ");
    int ch = fgetc(fp);
    if (ch != EOF) {
        printf("%c\n", ch);
        ungetc(ch, fp);
        printf("Unget character '%c' and re-read: %c\n", ch, fgetc(fp));
    }

    printf("\n");
    pauseConsole();
}

// 3. Formatted Input/Output
static void demonstrateFormattedIO(void) {
    printf("3. Formatted I/O Demonstration\n");

    // Append formatted data
    fprintf(fp, "Formatted number: %d\n", 42);
    fprintf(fp, "Formatted float: %.2f\n", 3.14159);
    fflush(fp);

    rewind(fp);
    printf("Reading formatted data back with fscanf:\n");

    char word[BUFFER_SIZE];
    int number;
    float fnum;
    // Example parse; skip until word "Formatted"
    while (fscanf(fp, "%127s", word) == 1) {
        if (strcmp(word, "Formatted") == 0) {
            fscanf(fp, "%*s %d", &number);  // skip "number:" then read int
            fscanf(fp, "%*s %f", &fnum);    // skip "Formatted" "float:" read float
            printf("Parsed number = %d, float = %.2f\n", number, fnum);
            break;
        }
    }

    printf("\n");
    pauseConsole();
}

// 4. File Positioning
static void demonstratePositioning(void) {
    printf("4. File Positioning Demonstration\n");

    rewind(fp);
    fpos_t pos;
    fgetpos(fp, &pos);

    printf("Current position recorded with fgetpos.\n");
    printf("Skipping 5 bytes with fseek.\n");

    fseek(fp, 5, SEEK_CUR);
    long current = ftell(fp);
    printf("Current file pointer (ftell): %ld\n", current);

    printf("Resetting position using fsetpos.\n");
    fsetpos(fp, &pos);

    printf("Pointer reset to start of file.\n\n");
    pauseConsole();
}

// 5. Error Handling
static void demonstrateErrorHandling(void) {
    printf("5. Error Handling Demonstration\n");

    clearerr(fp);
    int result = fgetc(fp);
    while (result != EOF) result = fgetc(fp);
    if (feof(fp))
        printf("Reached end of file.\n");
    else if (ferror(fp))
        perror("File error detected");

    clearerr(fp);
    printf("clearerr() used to reset file state.\n\n");
    pauseConsole();
}

// 6. File Operations
static void demonstrateFileOperations(void) {
    printf("6. File Operation Demonstration\n");

    safeClose(&fp);

    // Rename file
    if (rename(SAMPLE_FILE, COPY_FILE) == 0)
        printf("Renamed '%s' to '%s'.\n", SAMPLE_FILE, COPY_FILE);
    else
        perror("rename failed");

    // Open renamed file for reading
    errno_t err = fopen_s(&fp_copy, COPY_FILE, "r");
    if (err != 0 || !fp_copy) {
        perror("fopen_s read failed");
        return;
    }

    // Copy file to temp using fread/fwrite
    FILE *temp;
    err = fopen_s(&temp, "copy_out.txt", "wb");
    if (err != 0 || !temp) {
        perror("fopen_s write failed");
        safeClose(&fp_copy);
        return;
    }

    char buf[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buf, 1, sizeof(buf), fp_copy)) > 0) {
        fwrite(buf, 1, bytes, temp);
    }

    safeClose(&fp_copy);
    safeClose(&temp);

    printf("File copied successfully using fread/fwrite.\n");

    // Remove the copied files
    if (remove("copy_out.txt") == 0)
        printf("Temporary file removed.\n");
    if (remove(COPY_FILE) == 0)
        printf("Renamed file removed.\n");

    printf("\nAll file operations complete.\n");
}
