#include <engine/post.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int test_create_post(void* data)
{
    (void) data;
    int status = 1;
    Post *post = new_post("title", "name", "text", 1, 2);
    /* Make sure fields have been set correctly */
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->name, "name") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->reply_to == 2);
    /* Make sure fields have been guessed correctly */
    assert(! post->is_op);
    assert(post->len == strlen("text"));
    free_post(post);
    /* Test default name */
    post = new_post("title", NULL, "text", 1, 2);
    assert(strcmp(post->name, "Anonymous") == 0);
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->id == 1);
    assert(post->reply_to == 2);
    assert(! post->is_op);
    assert(post->len == strlen("text"));
    free_post(post);
    /* Create an OP post */
    post = new_post("title", "name", "text", 1, -1);
    assert(strcmp(post->name, "name") == 0);
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->id == 1);
    assert(post->reply_to == -1);
    assert(post->is_op);
    assert(post->len == strlen("text"));
    free_post(post);
    return status;
}

int test_posts()
{
    int status = 1;
    puts("Testing creating a post...");
    test_create_post(NULL);
    return status;
}

