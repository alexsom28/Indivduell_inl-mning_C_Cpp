#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "safeinput.h"


//  DATATYPER

typedef struct {
    int cardNumber;
    int hasAccess;
    char dateAdded[40];
} Card;

typedef struct {
    Card *cards;
    int count;
} SystemState;



//  HJÄLPFUNKTIONER

void getDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);
}

int findCard(SystemState *state, int number) {
    for (int i = 0; i < state->count; i++)
        if (state->cards[i].cardNumber == number)
            return i;
    return -1;
}



//  MENYFUNKTIONER

void remoteOpenDoor() {
    printf("\n Door is opening...\n");
    printf(" CURRENTLY LAMP: GREEN (Door Open)\n");
    printf("Simulating 3 seconds...\n\n");
}

void listAllCards(SystemState *state) {
    printf("\n REGISTERED CARDS:\n--------------------\n");

    if (state->count == 0) {
        printf("No cards registered in the system.\n\n");
        return;
    }

    for (int i = 0; i < state->count; i++) {
        printf("%d | %s | Added: %s\n",
            state->cards[i].cardNumber,
            state->cards[i].hasAccess ? "ACCESS" : "NO ACCESS",
            state->cards[i].dateAdded);
    }
    printf("\n");
}

void addRemoveAccess(SystemState *state) {
    int number = safeInt("\nEnter card number: ");
    int index = findCard(state, number);

    if (index == -1) {
        // Skapa nytt kort med realloc
        state->cards = realloc(state->cards, (state->count + 1) * sizeof(Card));
        state->cards[state->count].cardNumber = number;
        state->cards[state->count].hasAccess = 1;
        getDate(state->cards[state->count].dateAdded);
        state->count++;

        printf("Card %d was created and now has ACCESS.\n", number);
    } else {
        state->cards[index].hasAccess = !state->cards[index].hasAccess;
        printf("Card %d is now: %s\n",
            number,
            state->cards[index].hasAccess ? "ACCESS" : "NO ACCESS");
    }
}

void testScan(SystemState *state) {
    int number = safeInt("\nScan card number: ");
    int index = findCard(state, number);

    if (index == -1 || state->cards[index].hasAccess == 0)
        printf(" RED LAMP - Access Denied!\n\n");
    else
        printf(" GREEN LAMP - Access Granted!\n\n");
}



int main() {
    SystemState state = {NULL, 0}; 
    int choice;

    while (1) {
        printf("\n========= ADMIN MENU =========\n");
        printf("1. Remote open door\n");
        printf("2. List all cards\n");
        printf("3. Add/remove access\n");
        printf("4. Fake card test scanning\n");
        printf("5. Exit program\n");

        choice = safeInt("Choice: ");

        switch (choice) {
            case 1: remoteOpenDoor(); break;
            case 2: listAllCards(&state); break;
            case 3: addRemoveAccess(&state); break;
            case 4: testScan(&state); break;
            case 5:
                printf("Exiting program...\n");
                free(state.cards);
                return 0;
            default:
                printf("Invalid choice, try again!\n");
        }
    }
}
