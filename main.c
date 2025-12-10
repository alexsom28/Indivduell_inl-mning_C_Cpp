#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_CARDS 100
#define DOOR_PIN 1234
#define DB_FILE "cards.db"   // <-- Fil där kort sparas

typedef struct {
    int cardNumber;
    int hasAccess;
    char dateAdded[40];
} Card;

Card cardDB[MAX_CARDS];
int cardCount = 0;

// Hämtar dagens datum som text
void getDate(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// ----------------------------
// Ladda / spara kort i fil
// ----------------------------
void saveCardsToFile() {
    FILE *f = fopen(DB_FILE, "w");
    if (!f) {
        printf("Kunde inte spara databasen!\n");
        return;
    }

    for (int i = 0; i < cardCount; i++) {
        fprintf(f, "%d %d %s\n",
                cardDB[i].cardNumber,
                cardDB[i].hasAccess,
                cardDB[i].dateAdded);
    }

    fclose(f);
}

void loadCardsFromFile() {
    FILE *f = fopen(DB_FILE, "r");
    if (!f) {
        printf("Ingen databas hittades, skapar ny...\n");
        return;
    }

    cardCount = 0;
    while (fscanf(f, "%d %d %[^\n]",
                  &cardDB[cardCount].cardNumber,
                  &cardDB[cardCount].hasAccess,
                  cardDB[cardCount].dateAdded) == 3)
    {
        cardCount++;
        if (cardCount >= MAX_CARDS) break;
    }

    fclose(f);
    printf("Laddade %d kort från fil.\n", cardCount);
}

// Hitta kort
int findCard(int number) {
    for (int i = 0; i < cardCount; i++) {
        if (cardDB[i].cardNumber == number)
            return i;
    }
    return -1;
}

// ------------------------
// Remote open door + PIN
// ------------------------
void remoteOpenDoor() {
    int pin;

    printf("\n--- Remote Open Door ---\n");
    printf("Skriv PIN-kod: ");
    scanf("%d", &pin);

    if (pin != DOOR_PIN) {
        printf("Fel PIN! Dörren förblir stängd.\n\n");
        return;
    }

    printf("PIN OK!\n");
    printf("GREEN LIGHT ON (Door Open)\n");
    sleep(3);
    printf("GREEN LIGHT OFF (Door Closed)\n\n");
}

// Lista kort
void listAllCards() {
    printf("\n--- Registered Cards ---\n");
    if (cardCount == 0) {
        printf("Inga kort i systemet.\n\n");
        return;
    }
    for (int i = 0; i < cardCount; i++) {
        printf("Card: %d | Access: %s | Added: %s\n",
               cardDB[i].cardNumber,
               cardDB[i].hasAccess ? "YES" : "NO",
               cardDB[i].dateAdded);
    }
    printf("\n");
}

// Lägg till / ta bort access
void addRemoveAccess() {
    int number;
    printf("\nSkriv ett kortnummer: ");
    scanf("%d", &number);

    int index = findCard(number);

    if (index == -1) {
        printf("Kortet finns inte. Skapar nytt kort...\n");
        cardDB[cardCount].cardNumber = number;
        cardDB[cardCount].hasAccess = 1;
        getDate(cardDB[cardCount].dateAdded);
        cardCount++;

        printf("Kort %d skapades och har nu ACCESS.\n\n", number);
    } else {
        cardDB[index].hasAccess = !cardDB[index].hasAccess;
        printf("Kort %d: Access är nu %s.\n\n",
               number,
               cardDB[index].hasAccess ? "PÅ" : "AV");
    }

    saveCardsToFile();    // <-- Spara direkt
}

// Testscanna kort
void testScan() {
    int number;
    printf("\nTest scan - ange kortnummer: ");
    scanf("%d", &number);

    int index = findCard(number);

    if (index == -1 || cardDB[index].hasAccess == 0) {
        printf("RED LIGHT (Access Denied!)\n\n");
    } else {
        printf("GREEN LIGHT (Access Granted!)\n\n");
    }
}

int main() {
    loadCardsFromFile();  // <-- Ladda kort vid start

    int choice;

    while (1) {
        printf("======= Access Control System =======\n");
        printf("1. Remote open door\n");
        printf("2. List all cards\n");
        printf("3. Add/remove access\n");
        printf("4. Fake card test scanning\n");
        printf("5. Exit\n");
        printf("Val: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: remoteOpenDoor(); break;
            case 2: listAllCards(); break;
            case 3: addRemoveAccess(); break;
            case 4: testScan(); break;
            case 5:
                saveCardsToFile();   // <-- Spara innan avslut
                return 0;
            default:
                printf("Fel val!\n\n");
        }
    }

    return 0;
}
