/*
School Management System (C17)

Features:
- Add Student
- Find student by roll number
- Find student(s) by first name
- Find students by course
- Count students
- Delete student (by roll number)
- Update student (by roll number)
- Modular: functions above main
- static for internal linkage
- safe fgets/scanning and newline trimming

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

//  Constants & Macros 
#define MAX_STUDENTS 200

//  Struct Definitions
typedef struct Student {
    int roll_number;
    char first_name[50];
    char last_name[50];
    char course[50];
    float gpa;
} student_t;

//  Global Variables
static student_t students[MAX_STUDENTS];
static int total_students = 0;
static int next_roll_number = 1;

//  Function Declarations
static void clearInputBuffer(void);
static void trimNewline(char *string);
static void addStudent(void);
static void findByRoll(void);
static void findByFirstName(void);
static void findByCourse(void);
static void countStudents(void);
static void deleteStudent(void);
static void updateStudent(void);
static void printStudent(const student_t *student);
static void mainMenu(void);

//  Helpers Functions
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

static void trimNewline(char *string) {
    size_t len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
}

//  Driver Code
int main(void) {
    printf("School Management System (in-memory)\n");
    mainMenu();
    return 0;
}

//  Function Definitions
static void addStudent(void) {
    if (total_students >= MAX_STUDENTS) {
        printf("Student limit reached (%d). Cannot add more.\n", MAX_STUDENTS);
        return;
    }

    student_t st = {0};
    st.roll_number = next_roll_number++;

    printf("\n- Add Student -\n");
    clearInputBuffer();

    printf("First name: ");
    if (fgets(st.first_name, sizeof(st.first_name), stdin) == NULL) return;
    trimNewline(st.first_name);

    printf("Last name: ");
    if (fgets(st.last_name, sizeof(st.last_name), stdin) == NULL) return;
    trimNewline(st.last_name);

    printf("Course: ");
    if (fgets(st.course, sizeof(st.course), stdin) == NULL) return;
    trimNewline(st.course);


    printf("GPA (0.0 - 4.0): ");
    if (scanf_s("%f", &st.gpa) != 1) {
        clearInputBuffer();
        printf("Invalid GPA. Student not added.\n");
        return;
    }

    students[total_students++] = st;
    printf("Student added. Roll number: %d\n", st.roll_number);
}

//  Find by roll number
static void findByRoll(void) {
    int roll;
    printf("\nEnter roll number to search: ");
    if (scanf_s("%d", &roll) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < total_students; i++) {
        if (students[i].roll_number == roll) {
            printStudent(&students[i]);
            return;
        }
    }

    printf("No student found with roll number %d.\n", roll);
}

// Find by first name (case-sensitive substring exact match)
static void findByFirstName(void) {
    char query[50];
    printf("\nEnter first name to search: ");
    clearInputBuffer();
    if (fgets(query, sizeof(query), stdin) == NULL) return;
    trimNewline(query);

    int found = 0;
    for (int i = 0; i < total_students; i++) {
        if(strcmp(students[i].first_name, query) == 0) {
            printStudent(&students[i]);
            found = 1;
        }
    }

    if (!found) printf("No students found with first name '%s'. \n", query);

}

//  Find students by course
static void findByCourse(void) {
    char query[50];
    printf("\nEnter course to search: ");
    clearInputBuffer();
    if (fgets(query, sizeof(query), stdin) == NULL) return;
    trimNewline(query);

    int found = 0;
    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].course, query) == 0) {
            printStudent(&students[i]);
            found = 1;
        }
    }
    if (!found) printf("No students found in course '%s'.\n", query);
}

//  Count students
static void countStudents(void) {
    printf("\nTotal registered students: %d\n", total_students);
}

//  Delete student by roll number
static void deleteStudent(void) {
    int roll;
    printf("\nEnter roll number to delete: ");
    if (scanf_s("%d", &roll) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < total_students; i++) {
        if (students[i].roll_number == roll) { index = i; break; }
    }

    if (index == -1) {
        printf("No student with roll %d.\n", roll);
        return;
    }

    //  Shift array down to remove student
    for (int i = index; i < total_students - 1; i++) {
        students[i] = students[i + 1];
    }

    --total_students;
    printf("Student with roll %d deleted.\n", roll);
}

//  Update student (by roll number)
static void updateStudent(void) {
    int roll;
    printf("\nEnter roll number to update: ");
    if (scanf_s("%d", &roll) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < total_students; i++) {
        if (students[i].roll_number == roll) {index = i; break;}
    }


    if (index == -1) {
        printf("No student with roll %d.\n", roll);
        return;
    }

    student_t *student = &students[index];
    printf("\nUpdating student (leave blank to keep current)\n");
    clearInputBuffer();

    char buffer[64];

    printf("First name [%s]: ", student->first_name);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        trimNewline(buffer);
        if (buffer[0] != '\0')
            strncpy_s(student->first_name, sizeof(student->first_name), buffer, _TRUNCATE);
    }
    
    printf("Last name [%s]: ", student->last_name);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        trimNewline(buffer);
        if (buffer[0] != '\0')
            strncpy_s(student->last_name, sizeof(student->last_name), buffer, _TRUNCATE);
    }
    
    printf("Course [%s]: ", student->course);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        trimNewline(buffer);
        if (buffer[0] != '\0')
            strncpy_s(student->course, sizeof(student->course), buffer, _TRUNCATE);
    }

    printf("GPA [%.2f]: ", student->gpa);
    //  Read line and parse float optionally
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        trimNewline(buffer);
        if (buffer[0] != '\0') {
            float g;
            if (scanf_s(buffer, "%f", &g) == 1)
                student->gpa = g;
            else
                printf("Invalid GPA input; keeping old value.\n");
        }
    }

    printf("Student updated.\n");   
}

//  Print student details
static void printStudent(const student_t *student) {
    printf("\n-----------------------------\n");
    printf("Roll: %d\n", student->roll_number);
    printf("Name: %s %s\n",student->first_name, student->last_name);
    printf("Course: %s\n", student->course);
    printf("GPA: %.2f\n", student->gpa);
    printf("-----------------------------\n");
}

//  Main menu loop
static void mainMenu(void) {
    int choice;
    while(1) {
        printf("\n- SCHOOL MANAGEMENT SYSTEM -\n");
        printf("1. Add Student\n");
        printf("2. Find Student by Roll Number\n");
        printf("3. Find Student by First Name\n");
        printf("4. Find Students by Course\n");
        printf("5. Count Students\n");
        printf("6. Delete Student\n");
        printf("7. Update Student\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: findByRoll(); break;
            case 3: findByFirstName(); break;
            case 4: findByCourse(); break;
            case 5: countStudents(); break;
            case 6: deleteStudent(); break;
            case 7: updateStudent(); break;
            case 8: printf("Goodbye.\n"); return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}