#include "threads.c"
#include "posts.c"
#include "boards.c"
#include "rendering.c"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    test_posts();
    run_thread_tests();
    test_boards();
    test_rendering();
    return 0;
}

