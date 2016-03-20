#include <util/http.h>

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

/* don't print more than 80 characters */
#define MAX_BACKWARDS 40
#define MAX_FORWARDS  40

#include <string.h>

void point_error_in_line(char buffer[], int currentpos)
{
    int firstc = currentpos,
        lastc = currentpos,
        nbackwards = 0,
        nforwards = 1,
        i = 0,
        arrowpos = 0;
    char line[MAX_BACKWARDS + MAX_FORWARDS];
    /* find either where line starts or where buffer begins or max characters */
    while (firstc >= 0 && buffer[firstc] != '\n' && nbackwards <= MAX_BACKWARDS) {
        --firstc;
    }
    firstc ++;
    /* find either end of buffer or end or line or max characters */
    while (lastc >= 0 && buffer[lastc] != '\0' && buffer[lastc] != '\n' 
            && buffer[lastc] != '\r' && nforwards <= MAX_FORWARDS) {
        ++lastc;
    }
    lastc--;
    arrowpos = currentpos - firstc;
    /* write line */
    for (i = firstc; i < lastc; ++i) {
        line[i] = buffer[i];
    }
    puts(line);
    /* write meme bug pointing arrow */
    for (i = 0; i < MAX_BACKWARDS + MAX_FORWARDS; ++i) {
        if (i == arrowpos) {
            line[i] = '^';
        } else if (i == MAX_BACKWARDS + MAX_FORWARDS - 1) {
            line[i] = '\0';
        } else {
            line[i] = ' ';
        }
    }
    puts(line);
}

HTTP_Method get_HTTP_Method(char str[])
{
    if (strcmp(str, "GET") == 0) {
        return GET;
    } else if (strcmp(str, "HEAD") == 0) {
        return HEAD;
    } else if (strcmp(str, "POST") == 0) {
        return POST;
    } else if (strcmp(str, "PUT") == 0) {
        return PUT;
    } else if (strcmp(str, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(str, "TRACE") == 0) {
        return TRACE;
    } else if (strcmp(str, "OPTIONS") == 0) {
        return OPTIONS;
    } else if (strcmp(str, "CONNECT") == 0) {
        return CONNECT;
    } else if (strcmp(str, "PATCH") == 0) {
        return PATCH;
    } else {
        return INVALID_METHOD;
    }
}

