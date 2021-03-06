#include <engine/board.h>

#include <stdlib.h>
#include <string.h>

Board *new_board(char *name, char *fullname, short max_threads, short flags)
{
    Board *board = malloc(sizeof(Board));
    board->threads = malloc(sizeof(Thread*) * max_threads);
    board->flags = flags;
    strncpy(board->name, name, BOARD_NAME_LEN);
    strncpy(board->fullname, fullname, BOARD_FULLNAME_LEN);
    board->post_count = 0;
    board->max_threads = max_threads;
    board->first_thread = -1;
    board->last_thread = -1;
    return board;
}

void free_board(Board *board)
{
    int i, j;
    if (board->flags & BOARD_AUTO_FREE) {
        for (i = 0; i < board->max_threads; ++i) {
            free_post(board->threads[i]->op);
            for (j = 0; j < board->threads[i]->max_replies; ++j) {
                if (board->threads[i]->replies[j] != 0) {
                    free_post(board->threads[i]->replies[j]);
                }
            }
            free_thread(board->threads[i]);
        }
    }
    free(board->threads);
    free(board);
}

void add_thread_to_board(Board *board, Thread *thread)
{
    if (board->last_thread == -1 && board->first_thread == -1) { /* First thread on board */
        board->first_thread = 0;
        board->last_thread = 0;
        board->threads[board->first_thread] = thread;
    } else if (board->last_thread == board->max_threads -1 ||   /* queue is full */
               board->first_thread == board->last_thread + 1) {
        board->threads[board->first_thread] = thread;
        board->last_thread = board->first_thread;
        board->first_thread += 1;
        if (board->first_thread >= board->max_threads) {
            board->first_thread = 0;
        }
    } else {  /* queue is still not full */
        board->last_thread += 1;
        board->threads[board->last_thread] = thread;
    }
}

