#pragma once

#include "post.h"
#include "thread.h"
#include "board.h"

void read_post_bin_noalloc(Post *Post, int filed);
Post *read_post_bin(int filed);

void read_thread_bin_noalloc(Thread *thread, int filed);
Thread *read_thread_bin(int filed);

void read_board_bin_noalloc(Board* board, int filed);
Board *read_board_bin(int filed);

