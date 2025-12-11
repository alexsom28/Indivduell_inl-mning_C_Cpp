#include <stdio.h>
#include <string.h>
#include "safeinput.h"

// Rensar kvarvarande skräp i stdin
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Läser säkert ett heltal
int safeInt(const char *prompt) {
    int value;
    char line[100];

    while (1) {
        printf("%s", prompt);

        if (!fgets(line, sizeof(line), stdin)) {
            printf("Inmatningsfel, försök igen.\n");
            clearInputBuffer();
            continue;
        }

        // Försök tolka som heltal
        if (sscanf(line, "%d", &value) == 1) {
            return value;
        }

        printf("Felaktig inmatning, ange ett tal!\n");
    }
}

// Läser säkert en sträng
void safeString(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);

    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // ta bort \n
    } else {
        buffer[0] = '\0';
        clearInputBuffer();
    }
}
