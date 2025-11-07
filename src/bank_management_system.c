/*
Bank Management System in C17

Features:
- Struct-based data modeling
- Modular design and static internal linkage
- Binary file persistence
- Input validation and trimming helpers

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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define DATA_FILE "accounts.dat"

//  Struct for Account
typedef struct Account {
    int account_number;
    char name[50];
    char password[20];
    float balance;
} account_t;

//  Global Variables
static account_t accounts[MAX_ACCOUNTS];
static int total_accounts = 0;
static int next_account_number = 1000;

//  Function Declarations
static void loadAccountsFromFile(void);
static void saveAccountsToFile(void);
static void createAccount(void);
static int login(void);
static void checkBalance(account_t* account);
static void transferMoney(account_t* sender);
static void accountMenu(account_t* logged_in);
static void clearInputBuffer(void);
static void trimNewline(char* string);
static void mainMenu(void);

// Clear input buffer (flush stdin)
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

// Remove newline from fgets input
static void trimNewline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Driver Code
static void mainMenu(void) {
    int choice;
    while (1) {
        printf("\n----------------------------\n");
        printf("= BANK MANAGEMENT SYSTEM =\n");
        printf("----------------------------\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3:
                printf("Exiting...\n");
                saveAccountsToFile();
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

//  Function Definitions
static void loadAccountsFromFile(void) {
    FILE* file = NULL;
    errno_t error = fopen_s(&file, DATA_FILE, "rb");
    if (error != 0 || file == NULL) {
        printf("[INFO] No existing account data found. Starting fresh.\n");
        return;
    }

    if (fread(&total_accounts, sizeof(int), 1, file) != 1) {
        printf("[ERROR] Failed to read account count.\n");
        fclose(file);
        return;
    }

    if (fread(accounts, sizeof(account_t), total_accounts, file) != (size_t)total_accounts) {
        printf("[ERROR] Failed to read account data.\n");
        total_accounts = 0;
    }

    fclose(file);

    // Set next account number
    if (total_accounts > 0)
        next_account_number = accounts[total_accounts - 1].account_number + 1;

    printf("[INFO] Loaded %d account(s) from file.\n", total_accounts);
}

static void saveAccountsToFile(void) {
    FILE* file = NULL;
    errno_t error = fopen_s(&file, DATA_FILE, "wb");
    if (error != 0 || file == NULL) {
        printf("[ERROR] Unable to open file for writing.\n");
        return;
    }

    fwrite(&total_accounts, sizeof(int), 1, file);
    fwrite(accounts, sizeof(account_t), total_accounts, file);

    fclose(file);
    printf("[INFO] Saved %d account(s) to file.\n", total_accounts);
}

static void createAccount(void) {
    if (total_accounts >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts (limit reached).\n");
        return;
    }

    account_t new_account = {0};
    new_account.account_number = next_account_number++;

    printf("\n- Create New Account -\n");
    printf("Enter your name: ");
    clearInputBuffer();
    fgets(new_account.name, sizeof(new_account.name), stdin);
    trimNewline(new_account.name);

    printf("Set your password: ");
    fgets(new_account.password, sizeof(new_account.password), stdin);
    trimNewline(new_account.password);

    char confirm[20];
    printf("Confirm password: ");
    fgets(confirm, sizeof(confirm), stdin);
    trimNewline(confirm);

    if (strcmp(new_account.password, confirm) != 0) {
        printf("Passwords do not match. Account not created.\n");
        return;
    }

    printf("Enter initial deposit: ");
    if (scanf_s("%f", &new_account.balance) != 1 || new_account.balance < 0) {
        clearInputBuffer();
        printf("Invalid amount. Account not created.\n");
        return;
    }

    accounts[total_accounts++] = new_account;
    saveAccountsToFile();

    printf("\nAccount created successfully!\n");
    printf("Your account number is: %d\n", new_account.account_number);
}

static int login(void) {
    int entered_account;
    char entered_password[20];

    printf("\n= Login =\n");
    printf("Enter account number: ");
    if (scanf_s("%d", &entered_account) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return -1;
    }

    printf("Enter password: ");
    clearInputBuffer();
    fgets(entered_password, sizeof(entered_password), stdin);
    trimNewline(entered_password);

    for (int i = 0; i < total_accounts; i++) {
        if (accounts[i].account_number == entered_account &&
            strcmp(accounts[i].password, entered_password) == 0) {
            printf("\nLogin successful. Welcome, %s!\n", accounts[i].name);
            accountMenu(&accounts[i]);
            return i;
        }
    }

    printf("Login failed. Invalid account or password.\n");
    return -1;
}

static void checkBalance(account_t* account) {
    printf("\n= Account Balance =\n");
    printf("Name: %s\n", account->name);
    printf("Account No: %d\n", account->account_number);
    printf("Balance: $%.2f\n", account->balance);
}

static void transferMoney(account_t* sender) {
    int recipient_number;
    float amount;
    int recipient_index = -1;

    printf("\n= Transfer Money =\n");
    printf("Enter recipient account number: ");
    if (scanf_s("%d", &recipient_number) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < total_accounts; i++) {
        if (accounts[i].account_number == recipient_number) {
            recipient_index = i;
            break;
        }
    }

    if (recipient_index == -1) {
        printf("No account found with number %d.\n", recipient_number);
        return;
    }

    if (recipient_number == sender->account_number) {
        printf("You cannot transfer money to your own account.\n");
        return;
    }

    printf("Enter amount to transfer: ");
    if (scanf_s("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("Invalid amount.\n");
        return;
    }

    if (sender->balance < amount) {
        printf("Insufficient balance! Transaction canceled.\n");
        return;
    }

    sender->balance -= amount;
    accounts[recipient_index].balance += amount;
    saveAccountsToFile();

    printf("\nTransaction Successful!\n");
    printf("$%.2f transferred to %s (Account No: %d)\n",
           amount, accounts[recipient_index].name, recipient_number);
    printf("Your new balance: $%.2f\n", sender->balance);
}

static void accountMenu(account_t* logged_in) {
    int choice;
    while (1) {
        printf("\n--------------------------\n");
        printf("= ACCOUNT MENU = %s\n", logged_in->name);
        printf("----------------------------\n");
        printf("1. Check Balance\n");
        printf("2. Transfer Money\n");
        printf("3. Logout\n");
        printf("Enter choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: checkBalance(logged_in); break;
            case 2: transferMoney(logged_in); break;
            case 3:
                printf("Logging out...\n");
                saveAccountsToFile();
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
