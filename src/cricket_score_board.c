/*
Cricket Scoreboard System (Step 1)

Features:
- Define structures for Player, Team, and Match
- Declare all functions for future use
- Allow match setup (Team A vs Team B)
- Display initial scoreboard (all zeros)

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

#include <math.h>
#include <stdio.h>
#include <string.h>

//  Constants & Macros
#define MAX_PLAYERS 11

//  Struct Definitions
typedef struct Player {
    char name[50];
    int runs;
    int balls_faced;
    int wickets_taken;
} player_t;

typedef struct Team {
    char name[50];
    player_t players[MAX_PLAYERS];
    int total_runs;
    int wickets;
    float overs;
} team_t;

typedef struct Match {
    team_t teamA;
    team_t teamB;
    int current_innings; // 1 or 2
} match_t;

//  Function Declarations
static void clearInputBuffer(void);
static void setupMatch(match_t *match);
static void initializeTeam(team_t *team);
static void displayScoreboard(const match_t *match);
static void updateScore(match_t *match);
static void printStatistics(const match_t *match);
static void showResult(const match_t *match);

//  Utility Functions
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { ; }
}

//  Driver Code
int main(void) {
    match_t match;
    setupMatch(&match);
    displayScoreboard(&match);
    updateScore(&match);
    printStatistics(&match);
    showResult(&match);
    return 0;
}

//  Function Definitions
static void initializeTeam(team_t *team) {
    team->total_runs = 0;
    team->wickets = 0;
    team->overs = 0.0f;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        team->players[i].runs = 0;
        team->players[i].balls_faced = 0;
        team->players[i].wickets_taken = 0;
        snprintf(team->players[i].name, sizeof(team->players[i].name),
                 "Player %d", i + 1);
    }
}

static void setupMatch(match_t *match) {
    printf("\n= Cricket Match Setup =\n");
    printf("Enter Team A name: ");
    fgets(match->teamA.name, sizeof(match->teamA.name), stdin);
    match->teamA.name[strcspn(match->teamA.name, "\n")] = '\0';

    printf("Enter Team B name: ");
    fgets(match->teamB.name, sizeof(match->teamB.name), stdin);
    match->teamB.name[strcspn(match->teamB.name, "\n")] = '\0';

    initializeTeam(&match->teamA);
    initializeTeam(&match->teamB);

    match->current_innings = 1;

    printf("\nMatch between %s and %s successfully set up!\n",
           match->teamA.name, match->teamB.name);
}

static void displayScoreboard(const match_t *match) {
    printf("\n= CURRENT SCOREBOARD =\n");
    printf("%s: %d/%d (%.1f overs)\n", match->teamA.name,
           match->teamA.total_runs, match->teamA.wickets, match->teamA.overs);
    printf("%s: %d/%d (%.1f overs)\n", match->teamB.name,
           match->teamB.total_runs, match->teamB.wickets, match->teamB.overs);
}

static void updateScore(match_t *match) {
    int choice;
    int runs, wickets, balls;
    team_t *battingTeam;

    // Choose innings
    if (match->current_innings == 1)
        battingTeam = &match->teamA;
    else
        battingTeam = &match->teamB;

    printf("\nUpdating score for: %s (Innings %d)\n",
           battingTeam->name, match->current_innings);

    while (1) {
        printf("\n= Update Menu =\n");
        printf("1. Add Runs\n");
        printf("2. Add Wicket\n");
        printf("3. Add Balls\n");
        printf("4. Display Scoreboard\n");
        printf("5. End Innings\n");
        printf("Enter choice: ");

        if (scanf_s("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter runs scored: ");
            if (scanf_s("%d", &runs) == 1 && runs >= 0) {
                battingTeam->total_runs += runs;
                printf("Added %d runs.\n", runs);
            } else {
                clearInputBuffer();
                printf("Invalid input.\n");
            }
            break;

        case 2:
            printf("Enter number of wickets fallen: ");
            if (scanf_s("%d", &wickets) == 1 && wickets >= 0) {
                battingTeam->wickets += wickets;
                if (battingTeam->wickets > 10)
                    battingTeam->wickets = 10;
                printf("Updated wickets to %d.\n", battingTeam->wickets);
            } else {
                clearInputBuffer();
                printf("Invalid input.\n");
            }
            break;

        case 3:
            printf("Enter balls bowled: ");
            if (scanf_s("%d", &balls) == 1 && balls >= 0) {
                int overs_int = (int)battingTeam->overs;
                int current_balls = (int)((battingTeam->overs - overs_int) * 10);
                current_balls += balls;
                overs_int += current_balls / 6;
                current_balls %= 6;
                battingTeam->overs = overs_int + current_balls / 10.0f;
                printf("Updated overs to %.1f.\n", battingTeam->overs);
            } else {
                clearInputBuffer();
                printf("Invalid input.\n");
            }
            break;

        case 4:
            displayScoreboard(match);
            break;

        case 5:
            printf("Innings ended for %s.\n", battingTeam->name);
            match->current_innings++;
            return;

        default:
            printf("Invalid choice.\n");
        }
    }
}

static void printStatistics(const match_t *match) {
    printf("\n= MATCH STATISTICS =\n");

    const team_t *teams[2] = { &match->teamA, &match->teamB };
    for (int t = 0; t < 2; ++t) {
        printf("\nTeam: %s\n", teams[t]->name);
        printf("-------------------------------------\n");
        printf("%-15s %-10s %-10s %-10s\n",
               "Player", "Runs", "Balls", "Wickets");
        printf("-------------------------------------\n");

        for (int i = 0; i < MAX_PLAYERS; i++) {
            const player_t *p = &teams[t]->players[i];
            printf("%-15s %-10d %-10d %-10d\n",
                   p->name, p->runs, p->balls_faced, p->wickets_taken);
        }

        printf("-------------------------------------\n");
        printf("Total Runs: %d | Wickets: %d | Overs: %.1f\n",
               teams[t]->total_runs, teams[t]->wickets, teams[t]->overs);
    }
    printf("-------------------------------------\n");
}

static void showResult(const match_t *match) {
    printf("\n= MATCH RESULT =\n");
    const team_t *A = &match->teamA;
    const team_t *B = &match->teamB;

    if (A->total_runs > B->total_runs) {
        printf("%s won the match by %d runs!\n",
               A->name, A->total_runs - B->total_runs);
    } else if (B->total_runs > A->total_runs) {
        printf("%s won the match by %d runs!\n",
               B->name, B->total_runs - A->total_runs);
    } else {
        printf("The match between %s and %s ended in a draw!\n",
               A->name, B->name);
    }

    printf("\nFinal Scores:\n");
    displayScoreboard(match);
    printf("-------------------------------------\n");
}