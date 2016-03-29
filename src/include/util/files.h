#pragma once

#define MAX_LINE_BUFFER 10000

#include <stdlib.h>

void get_nextline(int filed, char **line);

void get_nextline_blocks(int filed, char *nextline[], char *extra[], char init[],
        size_t block_size); 

