#pragma once

#define N_POST_FIELDS 5 /* Do not forget to update when updating the post function */

#define POST_NFILES   0x00000008
#define POST_HAS_FILE 0x00000010
#define POST_SAGE     0x00000020

static const unsigned int MAX_POST_LENGTH = 2048;
static const unsigned int MAX_NAME_LENGTH = 128;
static const unsigned int MAX_TITLE_LENGTH = 128;

typedef struct {
    int is_op;
    char *name;  // field 0
    char *title; // field 1
    char *txt;   // field 2
    int len;
    int id;      // field 3
    int reply_to;// field 4
    int flags;   // field 5
} Post;

Post* new_post(char *title, char *name, char *txt, int id, int reply_to, int flags);
void free_post(Post* post); /* free from memory */
void save_post(Post* post); /* save to db */
void delete_post(Post* post); /* remove from db */

