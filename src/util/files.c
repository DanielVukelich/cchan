#include <util/files.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void get_nextline(int filed, char **nextline)
{
    int i, j, chars_read;
    int done = 0;
    size_t new_buffer_size;
    if (nextline == NULL) { /* trash a line */
        char c = 0;
        do {
            read(filed, &c, sizeof(char));
        } while (c != '\0' && c != '\n');
        return;
    }
    if (*nextline != NULL) {
        free(*nextline);
    }
    *nextline = malloc(sizeof(char) * 100); /* start with a buffer or 99 chars + '\0'*/
    while (! done) {
        for (i = 0; i < 99; ++i) {
            chars_read = read(filed, *nextline + i, sizeof(char));
            /* TODO: check for errors */
            if (chars_read <= 0) {
                (*nextline)[i] = '\0';
                return;
            }
            if ((*nextline)[i] == '\n') {/* check if line is complete */
                (*nextline)[i + 1] = '\0';
                return;
            }
        }
        /* if newline wasn't reached, allocate more buffer*/
        new_buffer_size = sizeof(char) * (j + 1) * 100;
        if (new_buffer_size > MAX_LINE_BUFFER) {
            /* TODO: set error code or something */
            return;
        }
        ++j;
        *nextline = realloc(*nextline, new_buffer_size);
    }
}

