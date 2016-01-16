#pragma once

#include "post.h"
#include "board.h"
#include "thread.h"

#include <stdlib.h>

extern char *open_post ,
            *close_div  ,
            *open_name  ,
            *open_title ,
            *close_span ,
            *open_header;

extern size_t open_post_len  ,
              close_div_len  ,
              open_name_len  ,
              open_title_len ,
              close_span_len ,
              open_header_len;

void init_rendering();
void render_post_html(Post *post, int filed);
void render_thread_html(Thread *thread, int filed);

