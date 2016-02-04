#include <util/strings.h>

#include <stdlib.h>
#include <string.h>

int str_alloc_and_copy(char **dst, char *src)
{
    size_t len = strlen(src);
    if (len == 0) {
        *dst = NULL;
        return 0;
    }
    *dst = malloc(len + 1);
    if (dst == NULL) {
        return -1;
    }
    strcpy(*dst, src);
    return 0;
}

