#include <io/render.h>
#include <engine/post.h>

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

int test_render_thread(void *data)
{
    (void) data;
    int status = 1;
    int fd, perms = S_IRUSR | S_IWUSR;
    /* create dummy objects */
    Post *op = new_post("TitleOP", "NameOP", "TextOP", 1, -1);
    Post *r1 = new_post("Title1", "Name1", "Text1", 2, 1);
    Post *r2 = new_post("Title2", "Name2", "Text2", 3, 1);
    Post *r3 = new_post("Title3", "Name3", "Text3", 4, 1);
    /* first with non cyclical thread */
    Thread *thread = new_thread(op, 2, 0);
    add_post_to_thread(thread, r1);
    add_post_to_thread(thread, r2);
    /* open file */
    fd = open("test_render_thread_noncyc", O_CREAT | O_RDWR, perms);
    render_thread_html(thread, fd);
    /* close and clean*/
    free_thread(thread);
    close(fd);
    /* now with a cyclical thread */
    thread = new_thread(op, 2, THREAD_CYCLIC);
    add_post_to_thread(thread, r1);
    add_post_to_thread(thread, r2);
    add_post_to_thread(thread, r3);
    fd = open("test_render_thread_cyc", O_CREAT | O_RDWR, perms);
    render_thread_html(thread, fd);
    /* clean and close */
    close(fd);
    free_post(op);
    free_post(r1);
    free_post(r2);
    free_post(r3);
    free_thread(thread);
    return status;
}

int test_render_thread_bin(void *data)
{
    (void) data;
    int status = 1;
    int perms = S_IRUSR | S_IWUSR;
    /* Create dummy posts */
    Post *post1 = new_post("Title1", "Name1", "Text1", 0, -1);
    Post *post2 = new_post("Title2", "Name2", "Text2", 1, 0);
    Thread *thread = new_thread(post1, 10, 0);
    add_post_to_thread(thread, post2);
    int filed = open("test_render_thread_bin", O_CREAT | O_RDWR, perms);
    render_thread_bin(thread, filed);
    close(filed);
    /* Clean stuff */
    free_post(post1);
    free_post(post2);
    free_thread(thread);
    return status;
}

int test_render_post_bin(void* data)
{
    (void) data;
    int status = 0;
    int perms = S_IWUSR | S_IRUSR;
    Post *post = new_post("Name", "Title", "Test", 0, -1);
    int filed = open("test_render_post_bin", O_CREAT | O_RDWR, perms);
    render_post_bin(post, filed);
    close(filed);
    free_post(post);
    return status;
}

int test_rendering()
{
    int status = 1;
    init_rendering();
    puts("Testing rendering a single post to HTML...");
    test_render_post(NULL);
    puts("Testing rendering a thread to HTML...");
    test_render_thread(NULL);
    puts("Testing rendering a thread to binary...");
    test_render_thread_bin(NULL);
    puts("Testing rendering a post to binary...");
    test_render_post_bin(NULL);
    return status;
}

