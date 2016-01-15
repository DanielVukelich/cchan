#pragma once

#define BOARD_NAME_LEN 32
#define BOARD_FULLNAME_LEN 96

#include "thread.h"

static const int MAX_THREADS_DEFAULT = 150;

typedef struct {
    char name[BOARD_NAME_LEN];
    char fullname[BOARD_FULLNAME_LEN];
    short max_threads;
    int post_count;
    /* Queue of threads */
    Thread **threads;
    short first_thread; /* Used in queue implementation */
    short last_thread;  /* same as above */
} Board;

Board *new_board(char *name, char *fullname, short max_threads);
void free_board(Board *board);
void add_thread_to_board(Board *board, Thread *thread);

