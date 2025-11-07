/*
Employee Management System (C17)

Features:
- Define employee structure and in-memory storage
- Display, add, delete, and search employee records
- Uses C17 safe functions (fgets_s, scanf_s, memcpy_s)
- Demonstrates modular layout and static linkage

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

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Constant Definition
#define MAX_EMPLOYEES 200

//  Struct Definitions
typedef struct Employee {
    int id;
    char name[50];
    char role[50];
    float salary;
} employee_t;

//  Global Variables
static employee_t employees[MAX_EMPLOYEES];
static int total_employees = 0;
static int next_id = 105;   //  Continue after preloaded employees

//  Function Declarations
static void clearInputBuffer(void);
static void trimNewline(char *string);
static void displayEmployees(void);
static void preloadEmployees(void);
static void addEmployee(void);
static void deleteEmployee(void);
static void searchEmployee(void);
static void mainMenu(void);

// Utility functions
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
    preloadEmployees();
    mainMenu();
    return 0;
}

//  Function Definitions
static void displayEmployees(void) {
    if (total_employees == 0) {
        printf("\nNo employees in the system.\n");
        return;
    }

    printf("\n- Employee Table -\n");
    printf("%-5s %-20s %-20s %-10s\n", "ID", "Name", "Role", "Salary");
    printf("-------------------------------------\n");

    for (int i = 0; i < total_employees; i++) {
        printf("%-5d %-20s %-20s %-10.2f\n",
               employees[i].id, employees[i].name, employees[i].role, employees[i].salary);
    }
}

static void preloadEmployees(void) {
    employee_t sample[] = {
        {101, "Alice Johnson", "Manager", 55000.00f},
        {102, "Bob Smith", "Developer", 42000.00f},
        {103, "Charlie Lee", "Desinger", 39000.00f},
        {104, "Diana Evans", "Tester", 36000.00f},
    };

    memcpy_s(employees, sizeof(employees), sample, sizeof(sample));
    total_employees = sizeof(sample) / sizeof(sample[0]);
}

static void addEmployee(void) {
    if (total_employees >= MAX_EMPLOYEES) {
        printf("Employee list full. Cannot add more employees.\n");
        return;
    }

    employee_t employee = {0};
    employee.id = next_id++;

    clearInputBuffer();
    printf("\n= Add New Employee =\n");

    printf("Name: ");
    if (fgets(employee.name, sizeof(employee.name), stdin) == NULL) return;
    trimNewline(employee.name);

    printf("Role: ");
    if (fgets(employee.role, sizeof(employee.role), stdin) == NULL) return;
    trimNewline(employee.role);

    printf("Salary: ");
    if (scanf_s("%f", &employee.salary) != 1) {
        clearInputBuffer();
        printf("Invalid input. Employee not added.\n");
        return;
    }

    employees[total_employees++] = employee;
    printf("Employee added successfully. Assigned ID: %d\n", employee.id);
}

static void deleteEmployee(void) {
    if (total_employees == 0) {
        printf("\nNo employees to delete.\n");
        return;
    }

    int id;
    printf("\nEnter Employee ID to delete: ");
    if (scanf_s("%d", &id) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter Employee ID to delete\n");
        return;
    }

    int found = -1;
    for (int i = 0; i < total_employees; i++) {
        if (employees[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    // Shift left to remove the record
    for (int i = found; i < total_employees - 1; i++) {
        employees[i] = employees[i + 1];
    }
    total_employees--;

    printf("Employee with ID %d deleted successfully.\n", id);
}

static void searchEmployee(void) {
    if (total_employees == 0) {
        printf("\nNo employees to search.\n");
        return;
    }

    int choice;
    printf("\n- Search Employee -\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("Enter your choice: ");
    if (scanf_s("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (choice == 1) {
        int id;
        printf("Enter Employee ID: ");
        if (scanf_s("%d", &id) != 1) {
            clearInputBuffer();
            printf("Invalid input.\n");
            return;
        }

        for (int i = 0; i < total_employees; i++) {
            if (employees[i].id == id) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nRole: %s\nSalary: %.2f\n",
                       employees[i].id, employees[i].name,
                       employees[i].role, employees[i].salary);
                return;
            }
        }
        
        printf("Employee with ID %d not found.\n", id);
    }
    else if (choice == 2) {
        clearInputBuffer();
        char search_name[50];
        printf("Enter name to search: ");
        if (fgets(search_name, sizeof(search_name), stdin) == NULL) return;
        trimNewline(search_name);

        int found = 0;
        printf("\nSearch Results:\n");
        for (int i = 0; i < total_employees; i++) {
            if (strstr(employees[i].name, search_name) != NULL) {
                printf("ID: %d | Name: %s | Role: %s | Salary: %.2f\n",
                       employees[i].id, employees[i].name,
                       employees[i].role, employees[i].salary);
                found = 1;
            }
        }
        if (!found)
            printf("No employees found matching '%s'.\n", search_name);
    }
    else {
        printf("Invalid choice.\n");
    }
}

//  Main menu
static void mainMenu(void) {
    int choice;

    while (1) {
        printf("\n= EMPLOYEE MANAGEMENT SYSTEM =\n");
        printf("1. Display All Employees\n");
        printf("2. Add New Employee\n");
        printf("3. Delete Employee\n");
        printf("4. Search Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: displayEmployees(); break;
            case 2: addEmployee(); break;
            case 3: deleteEmployee(); break;
            case 4: searchEmployee(); break;
            case 5: printf("Goodbye!\n"); return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}