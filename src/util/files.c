#include <util/files.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void get_nextline_blocks(int filed, char *nextline[], char *extra[], char init[], size_t block_size)
{
    int done = 0, read_bytes, line_size = 0, crlf_pos = 0;
    int nextl_start = 0, nextl_len = 0, init_len = 0;
    char *buffer, *aux;

    *nextline = NULL;
    /* check if there is anything in init */

    buffer = calloc(block_size, sizeof(char));
    if (init != NULL) {
        init_len = strlen(init);
        memcpy(buffer, init, init_len);
    }

    while (! done) {
        read_bytes = read(filed, buffer + init_len, block_size - init_len);
        init_len = 0;
        if (read_bytes < 0) {
            free(buffer);
            puts("Error reading from file");
            if (*nextline != NULL) {
                free(*nextline);
            }
            return;
        } else if ((unsigned int )read_bytes < block_size) {
            /* \r\n better be found at the end of the buffer */
            if (read_bytes < 2) {
                free(buffer);
                done = 1;
            } else if (buffer[read_bytes - 1] != '\n' || buffer[read_bytes - 2] != '\r') {
                free(buffer);
                done = 1;
            } else {
                /* k everything rite
                 * there is no extra buffer */
                buffer[read_bytes] = '\0';
                done = 1;
            }
        }

        /* look for \r */
        aux = strchr(buffer, '\r');
        if (aux != NULL) { /* found character */
            crlf_pos = aux - buffer;
            if ( *(aux + 1) != '\n') {
                free(buffer);
                free(*nextline);
                return;
            } else {
                done = 1;
                /* write extra stuff into extra buffer */
                nextl_start = crlf_pos + 2;
                nextl_len = read_bytes - nextl_start + 1;
                *extra = calloc(nextl_len, sizeof(char));
                memcpy(*extra, buffer + nextl_start, nextl_len - 1);
                /* write last piece of line */
                if (*nextline == NULL) {
                    *nextline = malloc(sizeof(char) * nextl_start + 1);
                    memcpy(*nextline, buffer, nextl_start);
                    line_size += 1;
                } else {
                    *nextline = realloc(*nextline, sizeof(char) * (nextl_start + line_size));
                    memcpy(*nextline + line_size, buffer, read_bytes);
                }
                line_size += nextl_start;
                nextline[0][line_size - 1] = '\0';
                return;
            }
        }

        if (*nextline == NULL) {
            *nextline = malloc(sizeof(char) * read_bytes);
        } else {
            *nextline = realloc(*nextline, sizeof(char) * (read_bytes + line_size));
        }
        memcpy(*nextline + line_size, buffer, read_bytes);
        line_size += read_bytes;
    }
    return;

}

