#include <util/http.h>

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

/* don't print more than 80 characters */
#define MAX_BACKWARDS 40
#define MAX_FORWARDS  40

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

