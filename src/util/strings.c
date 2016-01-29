#include <util/strings.h>

#include <stdlib.h>
#include <string.h>

int str_alloc_and_copy(char **dst, char *src)
{
    *dst = malloc(sizeof(char) * strlen(src));
    if (dst == NULL) {
        return -1;
    }
    strcpy(*dst, src);
    return 0;
}

