#include <util/strings.h>

#include <stdlib.h>
#include <string.h>

int str_alloc_and_copy(char **dst, char *src)
{
    size_t len;

    if (src == NULL) {
        *dst = NULL;
        return -1;
    }
    len = strlen(src) + 1;
    if (len == 0) {
        *dst = NULL;
        return 0;
    }
    *dst = malloc(len);
    if (dst == NULL) {
        return -1;
    }
    strcpy(*dst, src);
    return 0;
}

