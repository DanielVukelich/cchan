#include "include/post.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Post *new_post(char *title, char *name, char *txt, int id, int reply_to)
{
    Post *post = malloc(sizeof(Post));
    /* set name */
    post->name = malloc(sizeof(char) * MAX_NAME_LENGTH);
    if (name == NULL) { /* set "Anonymous" as a name */
        strncpy(post->name, "Anonymous", MAX_NAME_LENGTH);
    } else {
        strncpy(post->name, name, MAX_NAME_LENGTH);
    }
    /* set title */
    post->title = malloc(sizeof(char) * MAX_TITLE_LENGTH);
    if (title == NULL) { /* default title is empty title */
        post->title[0] = 0; /* set empty string */
    } else {
        strncpy(post->title, title, MAX_TITLE_LENGTH);
    }
    /* set text */
    if (txt == NULL) { /* place empty string */
        post->txt = malloc(sizeof(char));
        post->txt[0] = 0;
        post->len = 1;
    } else {
        size_t txtlen = strlen(txt);
        if (txtlen < MAX_POST_LENGTH) {
            post->txt = calloc(txtlen + 1, sizeof(char));
            post->len = txtlen;
            strncpy(post->txt, txt, txtlen);
        } else {
            post->txt = malloc(MAX_POST_LENGTH * sizeof(char));
            strncpy(post->txt, txt, MAX_POST_LENGTH);
            post->len = MAX_POST_LENGTH;
        }
    }
    /* set reply to */
    if (reply_to < 0) { /* post is OP */
        post->is_op = 1;
        post->reply_to = -1;
    } else { /* post is not OP */
        post->is_op = 0;
        post->reply_to = reply_to;
    }
    /* set id */
    post->id = id;
    return post;
}

void free_post(Post *post)
{
    free(post->txt);
    free(post->name);
    free(post->title);
    free(post);
}

