#include <engine/thread.h>
#include <engine/post.h>
#include <engine/board.h>

#include <stdio.h>
#include <assert.h>

/* Creates and empty thread. */
int test_empty_thread(void *data)
{
    (void) data;
    int status = 1; /* test is correct */
    Thread *thread = new_thread(NULL, 1, 0);
    /* Test that values have correctly been set */
    assert(thread != NULL);
    assert(thread->replies != NULL);
    assert(thread->op == NULL);
    assert(thread->max_replies == 1);
    assert(thread->first_post == -1);
    assert(thread->last_post == -1);
    assert(thread->nreplies == 0);
    free_thread(thread);
    return status;
}

int test_add_post(void *data)
{
    (void) data;
    int status = 1;
    /* create posts */
    Post *op = new_post("nameOP", "titleOP", "textOP", 0, -1);
    Post *reply = new_post("name", "title", "text", 1, 0);
    Thread *thread = new_thread(op, 150, 0);
    add_post_to_thread(thread, reply);
    assert(thread->nreplies == 1);
    assert(thread->first_post == 0);
    assert(thread->last_post == 0);
    free_thread(thread);
    free_post(op);
    free_post(reply);
    return status;
}

int test_thread_full(void* data) {
    (void) data;
    int status = 1;
    /* create dummy post */
    Post *post = new_post("title", "name", "text", 0, 0);
    Thread *thread = new_thread(NULL, 1, 0);
    add_post_to_thread(thread, post);
    add_post_to_thread(thread, post);
    assert(thread->nreplies == 1);
    free_thread(thread);
    free_post(post);
    return status;
}

int run_thread_tests() 
{
    int status = 1;
    puts("Testing empty threads...");
    test_empty_thread(NULL);
    puts("Testing adding post to thread...");
    test_add_post(NULL);
    puts("Testing adding post to full thread...");
    test_thread_full(NULL);
    return status;
}

