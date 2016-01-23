#pragma once

#include <engine/post.h>
#include <engine/board.h>
#include <engine/thread.h>

#include <stdlib.h>

extern char *open_post ,
            *open_postOP,
            *close_div  ,
            *open_name  ,
            *open_title ,
            *close_span ,
            *open_header;

extern size_t open_post_len  ,
              open_postOP_len,
              close_div_len  ,
              open_name_len  ,
              open_title_len ,
              close_span_len ,
              open_header_len;

void init_rendering();
void render_post_html(Post *post, int filed);
void render_post_bin(Post *post, int filed);
/* will happen much later
void render_post_json(Post *post, int filed); */

void render_thread_html(Thread *thread, int filed);
void render_thread_bin(Thread *thread, int filed);
/* will happen much later
void render_thread_json(Thread *thread, int filed); */

