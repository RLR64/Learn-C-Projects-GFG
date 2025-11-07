/*
Online Voting System (C17)

Features:
- Take input from user (vote casting)
- Store votes
- Calculate total votes
- Declare election results (handles ties)

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
#include <string.h>
#include <threads.h>

//  Constants & Macros
#define MAX_CANDIDATES 5


//  Struct Definition
typedef struct Candidate {
    char name[50];   // Candidate or Party name
    int votes;       // Vote count
} candidate_t;

static candidate_t candidates[MAX_CANDIDATES];
static int totalCandidates = 0;

//  Function Declarations
static void clearInputBuffer(void);
static void displayMenu(void);
static void takeInput(void);
static void storeVote(int choice);
static void calculateVotes(void);
static void declareResults(void);

//  Utility: Clear leftover input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//  Driver Code
int main(void) {
    //  Preload some candidates
    strcpy_s(candidates[0].name, sizeof(candidates[0].name), "Candidate A");
    strcpy_s(candidates[1].name, sizeof(candidates[1].name), "Candidate A");
    strcpy_s(candidates[2].name, sizeof(candidates[2].name), "Candidate A");
    totalCandidates = 3;

    int choice;

    do {
        displayMenu();
        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: takeInput(); break;
            case 2: calculateVotes(); break;
            case 3: declareResults(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

//  Function Definitions
static void displayMenu(void) {
    printf("\n= Online Voting System =\n");
    printf("1. Cast Vote\n");
    printf("2. View Total Votes\n");
    printf("3. View Results\n");
    printf("0. Exit\n");
    printf("Enter your choice");
}

static void takeInput(void) {
    int choice;

    printf("\n- Cast Your Vote -\n");
    for (int i = 0; i < totalCandidates; i++) {
        printf("%d. %s\n", i + 1, candidates[i].name);
    }
    printf("Enter your choice (1-%d): ", totalCandidates);

    if (scanf_s("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a valid number.\n");
        return;
    }

    if (choice < 1 || choice > totalCandidates) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    storeVote(choice);
}

static void storeVote(int choice) {
    candidates[choice - 1].votes++;
    printf("Vote recorded successfully for %s!\n\n", candidates[choice - 1].name);
}

static void calculateVotes(void) {
    int totalVotes = 0;

    printf("\n= Vote Summary =\n");
    for (int i = 0; i < totalCandidates; i++) {
        printf("%s: %d vote(s)\n", candidates[i].name, candidates[i].votes);
        totalVotes += candidates[i].votes;
    }

    printf("----------------------\n");
    printf("Total votes cast: %d\n\n", totalVotes);

    if (totalVotes == 0) {
        printf("No votes have been cast yet.\n\n");
    }
}

static void declareResults(void) {
    int highestVotes = 0;
    int totalVotes = 0;

    // Count total votes and find highest vote count
    for (int i = 0; i < totalCandidates; i++) {
        totalVotes += candidates[i].votes;
        if (candidates[i].votes > highestVotes) {
            highestVotes = candidates[i].votes;
        }
    }

    if (totalVotes == 0) {
        printf("\nNo votes have been cast yet. Results unavailable.\n\n");
        return;
    }

    printf("\n= Election Results =\n");

    //  Display each candidate's votes
    for (int i = 0; i < totalCandidates; i++) {
        printf("%s: %d vote(s)\n", candidates[i].name, candidates[i].votes);
    }

    printf("-----------------------------\n");

    // Count how many candidates have the highest votes (tie handling)
    int tieCount = 0;
    for (int i = 0; i < totalCandidates; i++) {
        if (candidates[i].votes == highestVotes) {
            tieCount++;
        }
    }

    //  Announce result
    if (tieCount > 1) {
        printf("It's a tie between:\n");
        for (int i = 0; i < totalCandidates; i++) {
            if (candidates[i].votes == highestVotes) {
                printf("- %s\n", candidates[i].name);
            }
        }
        printf("Each received %d vote(s).\n\n", highestVotes);
    } else {
        for (int i = 0; i < totalCandidates; i++) {
            if (candidates[i].votes == highestVotes) {
                printf("Winner: %s with %d vote(s)!\n\n",
                       candidates[i].name, candidates[i].votes);
                break;
            }
        }
    }
}