#pragma once

static const unsigned int MAX_POST_LENGTH = 2048;
static const unsigned int MAX_NAME_LENGTH = 128;
static const unsigned int MAX_TITLE_LENGTH = 128;

typedef struct {
    int is_op;
    char *name;
    char *title;
    char *txt;
    int len;
    int id;
    int reply_to;
} Post;

Post* new_post(char *title, char *name, char *txt, int id, int reply_to);
void free_post(Post* post); /* free from memory */
void save_post(Post* post); /* save to db */
void delete_post(Post* post); /* remove from db */

