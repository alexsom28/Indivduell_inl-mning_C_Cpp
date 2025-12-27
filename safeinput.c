#include "safeinput.h"
#include <stdio.h>
#include <string.h>

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int safeInt(const char *prompt) {
    int value;
    char line[100];

    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            printf("Fel vid läsning. Försök igen.\n");
            clearInputBuffer();
            continue;
        }
        if (sscanf(line, "%d", &value) == 1)
            return value;

        printf(" Ange ett giltigt tal!\n");
    }
}

void safeString(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
        clearInputBuffer();
    }
}
