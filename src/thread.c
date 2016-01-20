#include "include/thread.h"

#include <stdlib.h>

Thread *new_thread(Post* op, short max_replies, short flags)
{
    Thread *thread = malloc(sizeof(Thread));
    thread->op = op;
    thread->replies = malloc(sizeof(Post*) * max_replies);
    thread->max_replies = max_replies;
    thread->nreplies = 0;
    int i;
    for (i = 0; i < max_replies; ++i) {
        thread->replies[i] = NULL;
    }
    thread->flags = flags;
    thread->first_post = -1;
    thread->last_post = -1;
    return thread;
}

void free_thread(Thread *thread)
{
    if (thread->flags & THREAD_DELPOSTS) {
        short i;
        if (thread->flags & THREAD_CYCLIC) {
            if (thread->nreplies == thread->max_replies) {
                for (i = 0; i < thread->max_replies; ++i) {
                    free(thread->replies[i]);
                }
            }
            else if (thread->first_post < thread->last_post) {
                for (i = thread->first_post; i <= thread->last_post; ++i) {
                    free(thread->replies[i]);
                }
            } else {
                for (i = thread->first_post; i < thread->max_replies; ++i) {
                    free(thread->replies[i]);
                }
                for (i = 0; i <= thread->last_post; ++i) {
                    free(thread->replies[i]);
                }
            }
        } else {
            for (i = 0; i < thread->last_post + 1; ++i) {
                free(thread->replies[i]);
            }
        }
    }
    free(thread->replies);
    free(thread);
}

void add_post_to_thread(Thread *thread, Post *post)
{
    if (thread->nreplies >= thread->max_replies) {
        if (thread->flags & THREAD_CYCLIC) {
            /* replace oldest post by new post */
            thread->replies[thread->first_post] = post;
            /* update indices */
            if (thread->first_post >= thread->max_replies - 1) {
                thread->first_post = 0;
            } else {
                thread->first_post ++;
            }
            if (thread->last_post >= thread->max_replies - 1) {
                thread->last_post = 0;
            } else {
                thread->last_post ++;
            }
        }
        /* TODO: raise or log error, although this should never happen */
        return;
    }
    else {
        if (thread->nreplies < 1) {
            thread->first_post = 0;
        }
        thread->last_post ++;
        thread->nreplies ++;
        thread->replies[thread->last_post] = post;
    }
}

