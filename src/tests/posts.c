#include <engine/post.h>
#include <tests/pot.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int test_create_post(void* data)
{
    int status;
    int iter;
    unsigned int h_size;
    unsigned char *known_vect;
    unsigned char *name;
    unsigned char *pword;
    Post *post = new_post("title", "name", "text", 1, 2, 0);
    status = 1;
    (void) data;
    name = (unsigned char *) "name";
    pword = (unsigned char *) "password";
    /* Make sure fields have been set correctly */
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->name, "name") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->reply_to == 2);
    assert(post->trip.tripcode_hmac == NULL);
    assert(post->trip.hmac_size == 0);
    /* Make sure fields have been guessed correctly */
    assert(! post->is_op);
    assert(post->len == strlen("text"));
    free_post(post);
    /* Test default name */
    post = new_post("title", NULL, "text", 1, 2, 0);
    assert(strcmp(post->name, "Anonymous") == 0);
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->id == 1);
    assert(post->reply_to == 2);
    assert(! post->is_op);
    assert(post->len == strlen("text"));
    assert(post->trip.tripcode_hmac == NULL);
    assert(post->trip.hmac_size == 0);
    free_post(post);
    /* Create an OP post */
    post = new_post("title", "name", "text", 1, -1, 0);
    assert(strcmp(post->name, "name") == 0);
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->id == 1);
    assert(post->reply_to == -1);
    assert(post->is_op);
    assert(post->len == strlen("text"));
    assert(post->trip.tripcode_hmac == NULL);
    assert(post->trip.hmac_size == 0);
    free_post(post);
    /* Create a post with a tripcode */
    post = new_post("title", "name#password", "text", 1, 2, 0);
    assert(strcmp(post->name, "name") == 0);
    assert(strcmp(post->title, "title") == 0);
    assert(strcmp(post->txt, "text") == 0);
    assert(post->id == 1);
    assert(post->reply_to == 2);
    assert(! post->is_op);
    assert(post->len == strlen("text"));
    assert(post->trip.tripcode_hmac != NULL);
    
    known_vect = HMAC(EVP_sha512(), pword, 8, name, 4, NULL, &h_size);
      
    assert(post->trip.hmac_size == h_size);
    for(iter = 0; iter != 64; ++iter){
      assert(post->trip.tripcode_hmac[iter] == known_vect[iter]);
    }
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

