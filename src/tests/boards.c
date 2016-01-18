#include "../include/board.h"
#include "../include/thread.h"
#include "../include/post.h"

#include <assert.h>
#include <string.h>

int test_create_board(void *data)
{
    (void) data;
    int status = 1;
    Board *board = new_board("test", "Test", 1, 0);
    assert(board != NULL);
    assert(strcmp(board->name, "test") == 0);
    assert(strcmp(board->fullname, "Test") == 0);
    assert(board->max_threads == 1);
    assert(board->threads != NULL);
    assert(board->first_thread == -1);
    assert(board->last_thread == -1);
    assert(board->post_count == 0);
    free_board(board);
    return status;
}

int test_add_thread(void *data)
{
    (void) data;
    int status = 1;
    Board *board = new_board("test", "TEST", 2, 0);
    assert(board != NULL);
    Thread *thread = new_thread(NULL, 0, 0);
    /* Add the first thread */
    add_thread_to_board(board, thread);
    assert(board->first_thread == 0);
    assert(board->last_thread == 0);
    /* Add a second thread */
    add_thread_to_board(board, thread);
    assert(board->first_thread == 0);
    assert(board->last_thread == 1);
    /* Make sure the post count hasn't magically changed */
    assert(board->post_count == 0);
    /* Add a third thread, should remove the first one */
    add_thread_to_board(board, thread);
    assert(board->first_thread == 1);
    assert(board->last_thread == 0);
    /* Check post count again */
    assert(board->post_count == 0);
    free_board(board);
    free_thread(thread);
    return status;
}

int test_boards()
{
    int status = 1;
    puts("Testing creating a board...");
    test_create_board(NULL);
    test_add_thread(NULL);
    return status;
}

