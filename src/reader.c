#include "include/reader.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Assuming the post hasn't been freed at all,
 * you shouldn't create posts without new_post()
 * anyway */
void read_post_bin_noalloc(Post *post, int filed)
{
    /* Clean post */
    free(post->txt);
    /* Read from file */
    read(filed, &(post->id), sizeof(post->id));
    read(filed, &(post->reply_to), sizeof(post->reply_to));
    read(filed, post->name, MAX_NAME_LENGTH);
    read(filed, post->title, MAX_TITLE_LENGTH);
    read(filed, &(post->len), sizeof(post->len));
    post->txt = malloc(sizeof(char) * post->len);
    read(filed, &(post->txt), post->len);
    /* guess variables */
    if (post->reply_to < 0) {
        post->is_op = 1;
    } else {
        post->is_op = 0;
    }
}

Post *read_post_bin(int filed)
{
    /* TODO: this is poor performance but it avoids bugs tbqh */
    Post* post = new_post(" ", " ", " ", 0, 0);
    read_post_bin_noalloc(post, filed);
    return post;
}

/* same as with post, since you've created the thread with 
 * new_thread() (you have, right?), the thread contains
 * buffers of variables size that must be freed. */
void read_thread_bin_noalloc(Thread *thread, int filed)
{
    short i = 0;
    /* free replies buffers */
    if (thread->max_replies != 0) {
        free(thread->replies);
    }
    /* read stuff */
    read(filed, &(thread->flags), sizeof(thread->flags));
    read(filed, &(thread->max_replies), sizeof(thread->max_replies));
    /* allocate buffer for replies */
    thread->replies = malloc(sizeof(Post*) * thread->max_replies);
    /* read other options */
    read(filed, &(thread->nreplies), sizeof(thread->nreplies));
    /* set queue values */
    if (thread->flags & THREAD_CYCLIC) {
        thread->first_post = 0;
        thread->last_post = thread->nreplies - 1;
    } else {
        thread->first_post = thread->last_post = 0;
    }
    for (i = 0; i < thread->nreplies; ++i) {
            add_post_to_thread(thread, read_post_bin(filed));
    }
}

Thread *read_thread_bin(int filed)
{
    Thread *thread = new_thread(NULL, 0, 0);
    read_thread_bin_noalloc(thread, filed);
    return thread;
}

