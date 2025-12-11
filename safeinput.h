#ifndef SAFEINPUT_H
#define SAFEINPUT_H

void clearInputBuffer();
int safeInt(const char *prompt);
void safeString(const char *prompt, char *buffer, int size);

#endif
