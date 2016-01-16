#include "../include/render.h"
#include "../include/post.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int test_render_post(void *data)
{
    (void) data;
    int status = 1;
    /* open a test file */
    int file_perm_flags = S_IRUSR | S_IWUSR;
    int file = open("test_render_post", O_CREAT | O_RDWR, file_perm_flags);
    /* create a dummy post */
    Post *post = new_post("Title", "Name", "Text", 0, -1);
    /* write post on file */
    render_post_html(post, file);
    /* clean and close */
    free_post(post);
    close(file);
    return status;
}

int test_rendering()
{
    int status = 1;
    init_rendering();
    test_render_post(NULL);
    return status;
}

