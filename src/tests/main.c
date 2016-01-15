#include "threads.c"
#include "posts.c"
#include "boards.c"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    test_posts();
    run_thread_tests();
    test_boards();
    return 0;
}

