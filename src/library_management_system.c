/*
Library Management System (C17) - Final Step (with Borrower Tracking)

Features:
- Add Book
- Display All Books
- List Books by Author
- Count Total Books
- Issue Book to Student
- Return Book from Student
- List All Issued Books
- Exit

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

#include <corecrt.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 200
#define MAX_BORROWED 500
#define DATA_BOOKS "library.dat"
#define DATA_BORROW "borrowed.dat"

typedef struct Book {
    int id;
    char title[100];
    char author[50];
    int year;
    int copies;
} book_t;

typedef struct Borrow {
    int book_id;
    char student_name[50];
} borrow_t;

static book_t library[MAX_BOOKS];
static borrow_t borrowed[MAX_BORROWED];
static int total_books = 0;
static int total_borrowed = 0;
static int next_id = 1;

//  Utility functions
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

static void trimNewline(char *string) {
    size_t len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
}

//  Function Declarations
static void loadBooksFromFile(void);
static void saveBooksToFile(void);
static void loadBorrowedBooksFromFile(void);
static void saveBorrowedToFile(void);
static book_t *findBookById(int id);
static void addBook(void);
static void displayBooks(void);
static void listBooksByAuthor(void);
static void countBooks(void);
static void returnBook(void);
static void listIssuedBooks(void);
static void mainMenu(void);

//  Driver Code
int main(void) {
    printf("Library Manage System (C17)\n");
    loadBooksFromFile();
    loadBorrowedBooksFromFile();
    mainMenu();
    return 0;
}

//  Function Definitions
static void loadBooksFromFile(void) {
    FILE *file = NULL;
    errno_t error = fopen_s(&file, DATA_BOOKS, "rb");
    if (error != 0 || file == NULL) {
        printf("[INFO] No existing book data.\n");
        return;
    }
    fread(&total_books, sizeof(int), 1, file);
    fread(library, sizeof(book_t), total_books, file);
    fclose(file);

    int max_id = 0;
    for (int i = 0; i < total_books; ++i)
        if (library[i].id > max_id) max_id = library[i].id;
    next_id = max_id + 1;
}

static void saveBooksToFile(void) {
    FILE *file = NULL;
    errno_t error = fopen_s(&file, DATA_BOOKS, "wb");
    if (error != 0 || file == NULL) return;
    fwrite(&total_books, sizeof(int), 1, file);
    fwrite(library, sizeof(book_t), total_books, file);
    fclose(file);
}

static void loadBorrowedBooksFromFile(void) {
    FILE *file = NULL;
    errno_t error = fopen_s(&file, DATA_BORROW, "rb");
    if (error != 0 || file == NULL) {
        printf("[INFO] No existing borrow records.\n");
        return;
    }

    fread(&total_borrowed, sizeof(int), 1, file);
    fread(borrowed, sizeof(borrow_t), total_borrowed, file);
    fclose(file);
}

static void saveBorrowedToFile(void) {
    FILE *file = NULL;
    errno_t error = fopen_s(&file, DATA_BORROW, "wb");
    if (error != 0 || file == NULL) return;
    fwrite(&total_borrowed, sizeof(int), 1, file);
    fwrite(borrowed, sizeof(borrow_t), total_borrowed, file);
    fclose(file);
}

//  Find Book ID
static book_t *findBookById(int id) {
    for (int i = 0; i < total_books; ++i)
        if (library[i].id == id) return &library[i];
    return NULL;
}

//  Add Book
static void addBook(void) {
    if (total_books >= MAX_BOOKS) {
        printf("Library full.\n");
        return;
    }

    book_t book = {0};
    book.id = next_id++;

    clearInputBuffer();
    printf("\n- Add New Book-\n");


    printf("Title: ");
    fgets(book.title, sizeof(book.title), stdin);
    trimNewline(book.title);

    printf("Author: ");
    fgets(book.author, sizeof(book.author), stdin);
    trimNewline(book.author);

    printf("Year: ");
    if (scanf_s("%d", &book.year) != 1) {
        clearInputBuffer();
        printf("Invalid year.\n");
        return;
    }

    printf("Copies: ");
    if (scanf_s("%d", &book.copies) != 1) {
        clearInputBuffer();
        printf("Invalid copies count.\n");
        return;
    }

    library[total_books++] = book;
    saveBooksToFile();
    printf("Book added successfully. ID %d\n", book.id);
}

//  Display all books
static void displayBooks(void) {
    if (total_books == 0) {
        printf("\nNo books available.\n");
        return;
    }

    printf("\n- All Books -\n");
    for (int i = 0; i < total_books; ++i) {
        printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nCopies: %d\n",
               library[i].id, library[i].title,
               library[i].author, library[i].year, library[i].copies);
    }
}

//  List by author
static void listBooksByAuthor(void) {
    char query[50];
    printf("\nEnter author name: ");
    clearInputBuffer();
    fgets(query, sizeof(query), stdin);
    trimNewline(query);

    int found = 0;
    for (int i = 0; i < total_books; ++i) {
        if (strcmp(library[i].author, query) == 0) {
            printf("\nID: %d | %s (%d) — Copies: %d\n",
                   library[i].id, library[i].title, library[i].year, library[i].copies);
            found = 1;
        }
    }

    if (!found) printf("No Books by %s.\n", query);
}

//  Count books
static void countBooks(void) {
    printf("\nTotal books in library: %d\n", total_books);
}

// Issue Book
static void issueBook(void) {
    int id;
    char student[50];


    printf("\nEnter Book ID to issue: ");
    if (scanf_s("%d", &id) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }
 
    book_t *book = findBookById(id);

    if (!book) {
        printf("Books not found.\n");
        return;
    }

    if (book->copies <= 0) {
        printf("No copies available for '%s'.\n", book->title);
        return;
    }

    clearInputBuffer();
    printf("Enter student name: ");
    fgets(student, sizeof(student), stdin);
    trimNewline(student);

    if (total_borrowed >= MAX_BORROWED) {
        printf("Borrow list full.\n");
        return;
    }

    book->copies--;
    borrowed[total_borrowed].book_id = id;
    strcpy_s(borrowed[total_borrowed].student_name, sizeof(borrowed[total_borrowed].student_name), student);
    total_borrowed++;

    saveBooksToFile();
    saveBorrowedToFile();
    printf("Book issued to %s successfully.\n", student);
}

//  Return book
static void returnBook(void) {
    int id;
    char student[50];

    printf("\nEnter Book ID to return: ");
    if (scanf_s("%d", &id) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    clearInputBuffer();
    printf("Enter student name: ");
    fgets(student, sizeof(student), stdin);
    trimNewline(student);

    for (int i = 0; i < total_borrowed; ++i) {
        if (borrowed[i].book_id == id &&
            strcmp(borrowed[i].student_name, student) == 0) {
            // restore copies
            book_t *book = findBookById(id);
            if (book) book->copies++;

            // remove record
            borrowed[i] = borrowed[total_borrowed - 1];
            total_borrowed--;

            saveBooksToFile();
            saveBorrowedToFile();
            printf("Book returned successfully by %s.\n", student);
            return;
        }
    }

    printf("No matching borrow record found for %s.\n", student);
}

//  List all issued books
static void listIssuedBooks(void) {
    if (total_borrowed == 0) {
        printf("\nNo books currently issued.\n");
        return;
    }

    printf("\n- Issued Books -\n");
    for (int i = 0; i < total_borrowed; ++i) {
        book_t *book = findBookById(borrowed[i].book_id);
        if (book)
            printf("Book ID: %d | Title: %s | Borrowed by: %s\n",
                   book->id, book->title, borrowed[i].student_name);
    }
}

//  Menu
static void mainMenu(void) {
    int choice;

    while (1) {
        printf("\n= LIBRARY MANAGEMENT SYSTEM =\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. List Books by Author\n");
        printf("4. Count Books\n");
        printf("5. Issue Book to Student\n");
        printf("6. Return Book\n");
        printf("7. List All Issued Books\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again\n");
            continue;
        }

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: listBooksByAuthor(); break;
            case 4: countBooks(); break;
            case 5: issueBook(); break;
            case 6: returnBook(); break;
            case 7: listIssuedBooks(); break;
            case 8:
                printf("Exiting... All data saved.\n");
                saveBooksToFile();
                saveBorrowedToFile();
                return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}