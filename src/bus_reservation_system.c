/*
Bus Reservation System (C17)

Features:
- Admin login verification
- Access to main menu after successful login
- Clean modular structure with static functions

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

// Constants & Macros
#define MAX_BUSES 5
#define MAX_SEATS 40

//  Struct definitions
typedef struct Admin {
    char username[20];
    char password[20];
} admin_t;

typedef struct Booking {
    char passenger_name[50];
    int seat_number;
    int booked; // 0 = available, 1 = booked
} booking_t;

typedef struct Bus {
    int bus_id;
    char origin[50];
    char destination[50];
    char departure[20];
    char arrival[20];
    int total_seats;
    booking_t seats[MAX_SEATS];
} bus_t;

//  Global Variables
static admin_t admin_account = {"admin", "1234"};
static bus_t buses[MAX_BUSES];
static int total_buses = 3; //Preload 3 sample buses

//  Function Declarations
static void clearInputBuffer(void);
static int loginSystem(void);
static void mainMenu(void);
static void preloadBusData(void);
static void displayBuses(void);
static void bookTicket(void);
static void cancelTicket(void);
static void checkBusStatus(void);

//  Utility Function - Clear leftover input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

//  Driver Code
int main(void) {
    if (!loginSystem())
        return 0;   //  Login failed

    preloadBusData();  //   Initialize buses and seats
    mainMenu();
    return 0;
}


//  Function Definitions
static int loginSystem(void) {
    char username[20];
    char password[20];
    int attempts = 3;

    while (attempts) {
        printf("\n= BUS RESERVATION LOGIN =\n");
        printf("Username: ");
        scanf_s("%19s", username, (unsigned)_countof(username));
        printf("Password: ");
        scanf_s("%19s", password, (unsigned)_countof(password));

        if (strcmp(username, admin_account.username) == 0 &&
            strcmp(password, admin_account.password) == 0) {
            printf("\nLogin successful! Welcome %s\n", username);
            return 1;
        }

        attempts--;
        printf("Invalid credentials. Attempts left: %d\n", attempts);
    }

    printf("Access denied. Exiting program.\n");
    return 0;
}

static void preloadBusData(void) {
    bus_t sample[] = {
        {1, "Viremont", "Selbridge", "06:00 AM", "08:30 PM", 10, {0}},
        {2, "Dunhollow", "Astera", "09:00 AM", "05:00 PM", 10, {0}},
        {3, "Branwick", "Caldrith Gate", "07:30 AM", "03:30 PM", 10, {0}},
    };

    memcpy_s(buses, sizeof(buses), sample, sizeof(sample));

    // Initialize seat data
    for (int i = 0; i < total_buses; i++) {
        for (int j = 0; j < buses[i].total_seats; j++) {
            buses[i].seats[j].seat_number = j + 1;
            buses[i].seats[j].booked = 0;
            strcpy_s(buses[i].seats[j].passenger_name,
                     sizeof(buses[i].seats[j].passenger_name), "-");
        }
    }
}

static void displayBuses(void) {
    printf("\n- Available Buses -\n");
    printf("%-5s %-15s %-15s %-10s %-10s %-10s\n",
           "ID", "From", "To", "Depart", "Arrive", "Seats");
    for (int i = 0; i < total_buses; i++) {
        printf("%-5d %-15s %-15s %-10s %-10s %-10d\n",
               buses[i].bus_id, buses[i].origin, buses[i].destination,
               buses[i].departure, buses[i].arrival, buses[i].total_seats);
    }
}

static void bookTicket(void) {
    int bus_id, seat_number;
    char name[50];

    displayBuses();
    printf("\nEnter Bus ID to book: ");
    if (scanf_s("%d", &bus_id) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    bus_t *bus = NULL;
    for (int i = 0; i < total_buses; i++) {
        if (buses[i].bus_id == bus_id) {
            bus = &buses[i];
            break;
        }
    }

    if (!bus) {
        printf("Bus ID not found.\n");
        return;
    }

    printf("\nAvailable seats for Bus %d:\n", bus_id);
    for (int i = 0; i < bus->total_seats; i++) {
        if (!bus->seats[i].booked)
            printf("%d ", bus->seats[i].seat_number);
    }
    printf("\n");

    printf("Enter Seat Number to book a seat: ");
    if (scanf_s("%d", &seat_number) != 1 || seat_number < 1 || seat_number > bus->total_seats) {
        clearInputBuffer();
        printf("Invalid seat number.\n");
        return;
    }

    booking_t *seat = &bus->seats[seat_number - 1];
    if (seat->booked) {
        printf("Seat already booked.\n");
        return;
    }

    printf("Enter Passenger Name: ");
    clearInputBuffer();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Trim newline

    strcpy_s(seat->passenger_name, sizeof(seat->passenger_name), name);
    seat->booked = 1;

    printf("Ticket booked successfully!\n");
    printf("Passenger: %s | Bus; %s -> %s | Seat: %d\n",
        name, bus->origin, bus->destination, seat_number);

}

static void cancelTicket(void) {
    int bus_id, seat_number;

    displayBuses();
    printf("\nEnter Bus ID for cancellation: ");
    if (scanf_s("%d", &bus_id) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    bus_t *bus = NULL;
    for (int i = 0; i < total_buses; i++) {
        if (buses[i].bus_id == bus_id) {
            bus = &buses[i];
            break;
        }
    }

    if (!bus) {
        printf("Bus ID not found.\n");
        return;
    }

    printf("\nBooked seats for Bus %d:\n", bus_id);
    int bookedCount = 0;
    for (int i = 0; i < bus->total_seats; i++) {
        if (bus->seats[i].booked) {
            printf("Seat %2d - %s\n", bus->seats[i].seat_number, bus->seats[i].passenger_name);
            bookedCount++;
        }
    }

    if (bookedCount == 0) {
        printf("No seats currently booked on this bus.\n");
        return;
    }

    printf("\nEnter Seat Number to cancel: ");
    if (scanf_s("%d", &seat_number) != 1 || seat_number < 1 || seat_number > bus->total_seats) {
        clearInputBuffer();
        printf("Invalid seat number.\n");
        return;
    }

    booking_t *seat = &bus->seats[seat_number - 1];
    if (!seat->booked) {
        printf("That seat is not booked.\n");
        return;
    }

    printf("Cancelling booking for passenger: %s\n", seat->passenger_name);
    seat->booked = 0;
    strcpy_s(seat->passenger_name, sizeof(seat->passenger_name), "-");

    printf("Ticket successfully cancelled for seat %d.\n", seat_number);
}

static void checkBusStatus(void) {
    int bus_id;

    displayBuses();
    printf("\nEnter Bus ID to check status: ");
    if (scanf_s("%d", &bus_id) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    bus_t *bus = NULL;
    for (int i = 0; i < total_buses; i++) {
        if (buses[i].bus_id == bus_id) {
            bus = &buses[i];
            break;
        }
    }

    if (!bus) {
        printf("Bus ID not found.\n");
        return;
    }

    printf("\n= Bus Status =\n");
    printf("Route: %s => %s\n", bus->origin, bus->destination);
    printf("Departure: %s | Arrival: %s\n", bus->departure, bus->arrival);
    printf("Total Seats: %d\n\n", bus->total_seats);
    printf("%-10s %-20s\n", "Seat", "Status");
    printf("-----------------------------\n");

    int bookedCount = 0;
    for (int i = 0; i < bus->total_seats; i++) {
        booking_t *seat = &bus->seats[i];
        if (seat->booked) {
            printf("%-10d %-20s\n", seat->seat_number, seat->passenger_name);
            bookedCount++;
        } else {
            printf("%-10d %-20s\n", seat->seat_number, "Available");
        }
    }

    printf("\nTotal booked: %d | Available: %d\n",
           bookedCount, bus->total_seats - bookedCount);
}

//  Main menu
static void mainMenu(void) {
    int choice;

    while (1) {
        printf("\n= MAIN MENU =\n");
        printf("1. Book Tickets\n");
        printf("2. Cancel Tickets\n");
        printf("3. Check Bus Status\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: checkBusStatus(); break;
            case 4: printf("Goodbye!\n"); return;
            default: printf("Invalid choice.\n");
        }
    }
}