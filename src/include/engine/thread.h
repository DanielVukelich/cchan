#pragma once

#include "post.h"

#define THREAD_BUMPLOCKED 0x0001
#define THREAD_CYCLIC     0x0002
#define THREAD_DELPOSTS   0x0004 /* delete posts on free_thread? */

typedef struct {
    Post* op;
    Post** replies;
    short nreplies;
    /* options */
    short max_replies;
    short flags;
    short first_post; /* if cyclic, treat replies as a queue */
    short last_post;
} Thread;

Thread* new_thread(Post *op, short max_replies, short flags);

void free_thread(Thread *thread);

void save_thread(Thread *thread);

void delete_thread(Thread *thread);

void add_post_to_thread(Thread *thread, Post* post);

