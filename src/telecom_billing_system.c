/*
Telecom Billing System (C17)

Features:
- Add new records
- View list of records
- Modify records
- Search records
- Delete records
- View payment

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
#include <string.h>

//  File Name
#define DATA_FILE "records.dat"

//  Struct Definition
typedef struct Subscriber {
    char phone_number[20];
    char name[50];
    char address[100];
    float amount_due;
} subscriber_t;

//  Function Declarations
static void addNewRecord(void);
static void viewListOfRecords(void);
static void modifyRecord(void);
static void searchRecords(void);
static void deleteRecord(void);
static void viewPayment(void);
static void clearInputBuffer(void);
static void displayMenu(void);

//  Utility Function - Clear leftover input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//  Driver Code
int main(void) {
    int choice;

    do {
        displayMenu();

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: addNewRecord(); break;
            case 2: viewListOfRecords(); break;
            case 3: modifyRecord(); break;
            case 4: searchRecords(); break;
            case 5: deleteRecord(); break;
            case 6: viewPayment(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

//  Display Menu
static void displayMenu(void) {
    printf("\n= Telecom Billing System =\n");
    printf("1. Add New Record\n");
    printf("2. View List of Records\n");
    printf("3. Modify Record\n");
    printf("4. Search Records\n");
    printf("5. Delete Record\n");
    printf("6. View Payment\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

//  1. Add New Record
static void addNewRecord(void) {
    subscriber_t record;
    FILE *pfile = NULL;

    if (fopen_s(&pfile, DATA_FILE, "ab") != 0 || !pfile) {
        perror("Error opening file");
        return;
    }

    clearInputBuffer();
    printf("\n- Add New Subscriber -\n");

    printf("Enter name: ");
    fgets(record.name, sizeof(record.name), stdin);
    record.name[strcspn(record.name, "\n")] = '\0';

    printf("Enter phone number: ");
    fgets(record.phone_number, sizeof(record.phone_number), stdin);
    record.phone_number[strcspn(record.phone_number, "\n")] = '\0';

    printf("Enter address: ");
    fgets(record.address, sizeof(record.address), stdin);
    record.address[strcspn(record.address, "\n")] = '\0';

    printf("Enter amount due: ");
    if (scanf_s("%f", &record.amount_due) != 1) {
        clearInputBuffer();
        printf("Invalid amount entered.\n");
        fclose(pfile);
        return;
    }

    fwrite(&record, sizeof(subscriber_t), 1, pfile);
    fclose(pfile);

    printf("\nRecord added successfully!\n");
}

//  2. View List of Records
static void viewListOfRecords(void) {
    FILE *pfile = NULL;

    if (fopen_s(&pfile, DATA_FILE, "rb") != 0 || !pfile) {
        printf("\nNo records found. File missing or empty.\n");
        return;
    }

    subscriber_t record;
    int count = 0;

    printf("\n- List of Subscribers -\n");
    printf("%-20s %-20s %-25s %-10s\n", "Phone Number", "Name", "Address", "Amount");
    printf("\n---------------------------------\n");

    while (fread(&record, sizeof(subscriber_t), 1, pfile) == 1) {
        printf("%-20s %-20s %-25s %.2f\n",
               record.phone_number, record.name, record.address, record.amount_due);
        count++;
    }

    if (count == 0)
        printf("No records available.\n");

    fclose(pfile);
}

//  3. Modify Record
static void modifyRecord(void) {
    FILE *pfile = NULL;
    if (fopen_s(&pfile, DATA_FILE, "rb+") != 0 || !pfile) {
        printf("\nError opening file for modification.\n");
        return;
    }

    char phone[20];
    subscriber_t record;
    int found = 0;

    clearInputBuffer();
    printf("\nEnter phone number to modify: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';

    while (fread(&record, sizeof(subscriber_t), 1, pfile) == 1) {
        if (strcmp(record.phone_number, phone) == 0) {
            found = 1;
            printf("\nRecord found for %s\n", record.name);
            printf("Enter new name: ");
            fgets(record.name, sizeof(record.name), stdin);
            record.name[strcspn(record.name, "\n")] = '\0';

            printf("Enter new address: ");
            fgets(record.address, sizeof(record.address), stdin);
            record.address[strcspn(record.address, "\n")] = '\0';

            printf("Enter new amount due: ");
            if (scanf_s("%f", &record.amount_due) != 1) {
                clearInputBuffer();
                printf("Invalid amount.\n");
                fclose(pfile);
                return;
            }

            // Move file pointer back one record and overwrite
            fseek(pfile, -(long)sizeof(subscriber_t), SEEK_CUR);
            fwrite(&record, sizeof(subscriber_t), 1, pfile);
            printf("\nRecord updated successfully.\n");
            break;
        }
    }

    if (!found)
        printf("\nNo record found with that phone number.\n");

    fclose(pfile);
}

//  4. Search Records
static void searchRecords(void) {
    FILE *pfile = NULL;
    if (fopen_s(&pfile, DATA_FILE, "rb") != 0 || !pfile) {
        printf("\nError opening file for search.\n");
        return;
    }

    char phone[20];
    subscriber_t record;
    int found = 0;

    clearInputBuffer();
    printf("\nEnter phone number to search: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';

    while (fread(&record, sizeof(subscriber_t), 1, pfile) == 1) {
        if (strcmp(record.phone_number, phone) == 0) {
            printf("\nRecord found:\n");
            printf("Name: %s\n", record.name);
            printf("Phone: %s\n", record.phone_number);
            printf("Address: %s\n", record.address);
            printf("Amount Due: %.2f\n", record.amount_due);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nNo record found with that phone number.\n");

    fclose(pfile);
}

//  5. Delete Record
static void deleteRecord(void) {
    FILE *pfile = NULL, *temp = NULL;
    if (fopen_s(&pfile, DATA_FILE, "rb") != 0 || !pfile) {
        printf("\nError opening file for deletion.\n");
        return;
    }

    if (fopen_s(&temp, "temp.dat", "wb") != 0 || !temp) {
        printf("\nError creating temporary file.\n");
        fclose(pfile);
        return;
    }

    char phone[20];
    subscriber_t record;
    int found = 0;

    clearInputBuffer();
    printf("\nEnter phone number to delete: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';

    while (fread(&record, sizeof(subscriber_t), 1, pfile) == 1) {
        if (strcmp(record.phone_number, phone) != 0) {
            fwrite(&record, sizeof(subscriber_t), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(pfile);
    fclose(temp);

    if (found) {
        remove(DATA_FILE);
        rename("temp.dat", DATA_FILE);
        printf("\nRecord with phone number %s deleted successfully.\n", phone);
    } else {
        remove("temp.dat");
        printf("\nNo record found with that phone number.\n");
    }
}

//  6. View Payment
static void viewPayment(void) {
    FILE *pfile = NULL;
    if (fopen_s(&pfile, DATA_FILE, "rb+") != 0 || !pfile) {
        printf("\nError opening file for payment.\n");
        return;
    }

    char phone[20];
    subscriber_t record;
    int found = 0;

    clearInputBuffer();
    printf("\nEnter phone number to view payment: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';

    while (fread(&record, sizeof(subscriber_t), 1, pfile) == 1) {
        if (strcmp(record.phone_number, phone) == 0) {
            found = 1;
            printf("\n= Payment Details =\n");
            printf("Name: %s\n", record.name);
            printf("Phone: %s\n", record.phone_number);
            printf("Address: %s\n", record.address);
            printf("Current Amount Due: %.2f\n", record.amount_due);

            char choice;
            printf("\nWould you like to make a payment? (y/n): ");
            scanf_s(" %c", &choice, 1);

            if (choice == 'y' || choice == 'Y') {
                float payment;
                printf("Enter payment amount: ");
                if (scanf_s("%f", &payment) != 1 || payment <= 0) {
                    clearInputBuffer();
                    printf("Invalid amount.\n");
                    fclose(pfile);
                    return;
                }

                if (payment > record.amount_due) {
                    printf("Payment exceeds amount due. Transaction cancelled.\n");
                } else {
                    record.amount_due -= payment;
                    fseek(pfile, -(long)sizeof(subscriber_t), SEEK_CUR);
                    fwrite(&record, sizeof(subscriber_t), 1, pfile);
                    printf("Payment successful! Remaining balance: %.2f\n", record.amount_due);
                }
            }
            break;
        }
    }

    if (!found)
        printf("\nNo record found with that phone number.\n");

    fclose(pfile);
}