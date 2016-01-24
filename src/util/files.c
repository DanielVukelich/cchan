#include <util/files.h>
#include <unistd.h>

void get_nextline(int filed, char **nextline)
{
    int i, j;
    int done = 0;
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
        for (int i = 0; i < 99; ++i) {
            read(filed, *nextline + i, sizeof(char));
            if (*nextline[i] == EOF) 
                return;
            }
            if (*nextline[i] == '\n') {/* check if line is complete */
                *nextline[i + 1] = '\0';
                return;
            }
        }
        /* if newline wasn't reached, allocate more buffer*/
        size_t new_buffer_size = sizeof(char) * (j + 1) * 100;
        if (new_buffer_size > MAX_LINE_BUFFER) {
            /* TODO: set error code or something */
            return;
        }
        ++j;
        *nextline = realloc(*nextline, new_buffer_size);
    }
}

