/*
Hospital Management System (C17)

Features:
- Define hospital & patient structures
- Store data for ONE hospital
- Preload patient data
- Display hospital info and patient list

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
#define MAX_PATIENTS 200

// Struct Definitions
typedef struct Patient {
    int id;
    char name[50];
    int age;
    char disease[80];
    float bill_amount;
    int admitted;   // 1 = admitted, 0 = discharged
} patient_t;

typedef struct Hospital {
    char name[60];
    char city[40];
    int total_beds;
    int available_beds;
    float bed_price;
    float rating;
    int reviews;
    patient_t patients[MAX_PATIENTS];
    int total_patients;
} hospital_t;

//  Global Variables
static hospital_t hospital;

//  Function Declarations
static void clearInputBuffer(void);
static void trimNewline(char *string);
static void preloadData(void);
static void displayHospitalInfo(void);
static void displayPatients(void);
static void printPatientData(const patient_t *p);
static void sortByBillAmount(void);
static void sortByAvailableBeds(void);
static void sortByBedPrice(void);
static void sortByName(void);
static void sortByRating(void);
static void printPatientsOfSpecificCondition(void);
static void mainMenu(void);

//  Utility Function - Clear leftover input
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
    preloadData();
    mainMenu();
    return EXIT_SUCCESS;
}

//  Function Definitions
static void preloadData(void) {
    strcpy_s(hospital.name, sizeof(hospital.name), "Evergreen Medical Center");
    strcpy_s(hospital.city, sizeof(hospital.city), "Riverview");
    hospital.total_beds = 50;
    hospital.available_beds = 18;
    hospital.bed_price = 1500.0f;
    hospital.rating = 4.6f;
    hospital.reviews = 340;
    hospital.total_patients = 5;

    patient_t samples[] = {
        {101, "Alice Martinez", 34, "Flu", 1200.0f, 1},
        {102, "David Benton", 41, "Broken Arm", 5500.0f, 1},
        {103, "Naledi Dlamini", 28, "Fever", 950.0f, 1},
        {104, "Alice Mokoena", 50, "Heart Disease", 12000.0f, 1},
        {105, "Karen Moodley", 60, "Diabetes", 7000.0f, 1},
    };

    memcpy_s(hospital.patients, sizeof(hospital.patients), samples, sizeof(samples));
}

static void displayHospitalInfo(void) {
    printf("\n= Hospital Information =\n");
    printf("Name           : %s\n", hospital.name);
    printf("City           : %s\n", hospital.city);
    printf("Total Beds     : %d\n", hospital.total_beds);
    printf("Available Beds : %d\n", hospital.available_beds);
    printf("Bed Price      : %.2f\n", hospital.bed_price);
    printf("Rating         : %.1f (%d reviews)\n",
           hospital.rating, hospital.reviews);
}

static void printPatientData(const patient_t *p) {
    printf("%-5d %-20s %-5d %-20s %-10.2f %-10s\n",
           p->id, p->name, p->age, p->disease,
           p->bill_amount, p->admitted ? "Admitted" : "Discharged");
}

static void displayPatients(void) {
    if (hospital.total_patients == 0) {
        printf("\nNo patients in the hospital.\n");
        return;
    }

    printf("\n= Patients Data =\n");
    printf("%-5s %-20s %-5s %-20s %-10s %-10s\n",
           "ID", "Name", "Age", "Disease", "Bill", "Status");
           printf("\n---------------------------------\n");
    for (int i = 0; i < hospital.total_patients; i++)
        printPatientData(&hospital.patients[i]);
}

static void sortByBillAmount(void) {
    if (hospital.total_patients == 0) return;

    for (int i = 0; i < hospital.total_patients - 1; i++) {
        for (int j = 0; j < hospital.total_patients - i - 1; j++) {
            if (hospital.patients[j].bill_amount > hospital.patients[j + 1].bill_amount) {
                patient_t temp = hospital.patients[j];
                hospital.patients[j] = hospital.patients[j + 1];
                hospital.patients[j + 1] = temp;
            }
        }
    }

    printf("\nPatients sorted by Bill Amount (Low => High)\n");
    displayPatients();
}

static void sortByBedPrice(void) {
    printf("\n= Sort by Bed Price =\n");
    printf("Hospital       : %s\n", hospital.name);
    printf("Bed Price      : %.2f\n", hospital.bed_price);
    printf("(Single hospital — sorting simulated.)\n");
}

static void sortByAvailableBeds(void) {
    printf("\n= Sort by Available Beds =\n");
    printf("Hospital Name  : %s\n", hospital.name);
    printf("Total Beds     : %d\n", hospital.total_beds);
    printf("Available      : %d\n", hospital.available_beds);
    printf("Occupancy      : %.1f%% full\n",
           100.0f * (hospital.total_beds - hospital.available_beds) / hospital.total_beds);
    printf("(Single hospital — sorting simulated.)\n");
}

static void sortByName(void) {
    if (hospital.total_patients == 0) return;

    for (int i = 0; i < hospital.total_patients - 1; i++) {
        for (int j = 0; j < hospital.total_patients - i - 1; j++) {
            if (strcmp(hospital.patients[j].name, hospital.patients[j + 1].name) > 0) {
                patient_t temp = hospital.patients[j];
                hospital.patients[j] = hospital.patients[j + 1];
                hospital.patients[j + 1] = temp;
            }
        }
    }

    printf("\nPatients sorted by Name (A → Z)\n");
    displayPatients();
}

static void sortByRating(void) {
    printf("\n= Sort by Rating & Reviews =\n");
    printf("Hospital Name  : %s\n", hospital.name);
    printf("Rating         : %.1f\n", hospital.rating);
    printf("Reviews        : %d\n", hospital.reviews);
    printf("(Single hospital — sorting simulated.)\n");
}

static void printPatientsOfSpecificCondition(void) {
    if (hospital.total_patients == 0) {
        printf("\nNo patients in the hospital.\n");
        return;
    }

    char disease_query[80];
    printf("\nEnter disease name to filter (e.g., Fever): ");
    clearInputBuffer();
    fgets(disease_query, sizeof(disease_query), stdin);
    trimNewline(disease_query);

    printf("\n= Patients with '%s' =\n", disease_query);
    int found = 0;

    for (int i = 0; i < hospital.total_patients; i++) {
        if (_stricmp(hospital.patients[i].disease, disease_query) == 0) {
            printPatientData(&hospital.patients[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No patients found with disease: %s\n", disease_query);
}

// Main Menu
static void mainMenu(void) {
    int choice;

    while (1) {
        printf("\n= HOSPITAL MANAGEMENT SYSTEM =\n");
        printf("1. Print Hospital Data\n");
        printf("2. Print Patient Data\n");
        printf("3. Sort by Bill Amount\n");
        printf("4. Sort by Available Beds\n");
        printf("5. Sort by Bed Price\n");
        printf("6. Sort by Name\n");
        printf("7. Sort by Rating & Reviews\n");
        printf("8. Find Patients by Disease\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: displayHospitalInfo(); break;
            case 2: displayPatients(); break;
            case 3: sortByBillAmount(); break;
            case 4: sortByAvailableBeds(); break;
            case 5: sortByBedPrice(); break;
            case 6: sortByName(); break;
            case 7: sortByRating(); break;
            case 8: printPatientsOfSpecificCondition(); break;
            case 9: printf("Goodbye!\n"); return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}