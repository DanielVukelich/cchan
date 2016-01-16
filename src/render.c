#include "include/render.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *open_post = "<div class=\"post\">",
     *open_postOP="<div class=\"post op\">",
     *close_div = "</div>",
     *open_name = "<span class=\"name\">",
     *open_title= "<span class=\"title\">",
     *close_span= "</span>",
     *open_header="<div class=\"header\">";

size_t open_post_len  ,
       open_postOP_len,
       close_div_len  ,
       open_name_len  ,
       open_title_len ,
       close_span_len ,
       open_header_len;

void init_rendering()
{
    open_postOP_len = strlen(open_postOP);
    open_post_len = strlen(open_post);
    close_div_len = strlen(close_div);
    open_name_len = strlen(open_name);
    open_title_len= strlen(open_title);
    close_span_len= strlen(close_span);
    open_header_len=strlen(open_header);
}

void render_post_html(Post *post, int filed)
{
    /* some variables */
    size_t name_len = strlen(post->name),
           title_len= strlen(post->title);
    /* open div */
    if (post->is_op) {
        write(filed, open_postOP, open_postOP_len);
    } else {
        write(filed, open_post, open_post_len);
    }
    /* write header */
    write(filed, open_header, open_header_len);
    /* write poster name */
    write(filed, open_name, open_name_len);
    write(filed, post->name, name_len);
    write(filed, close_span, close_span_len);
    /* write post title */
    write(filed, open_title, open_title_len);
    write(filed, post->title, title_len);
    write(filed, close_span, close_span_len);
    /* close header */
    write(filed, close_div, close_div_len);
    /* write post text */
    write(filed, post->txt, post->len);
    /* close div */
    write(filed, close_div, close_div_len);
}

void render_thread_html(Thread *thread, int filed)
{
    int i;
    /* write OP */
    render_post_html(thread->op, filed);
    /* render all replies */
    if (thread->flags & THREAD_CYCLIC) {
        for (i = thread->first_post; i < thread->max_replies; ++i) {
            render_post_html(thread->replies[i], filed);
        }
        for (i = 0; i < thread->first_post; ++i) {
            render_post_html(thread->replies[i], filed);
        }
    } else {
        for (i = 0; i < thread->nreplies; ++i) {
            render_post_html(thread->replies[i], filed);
        }
    }
}

