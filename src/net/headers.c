#include <net/headers.h>
#include <util/strings.h>

#include <string.h>
#include <stdlib.h>

int parse_http_Host(http_Host *host, char value[])
{
    char *aux;
    aux = strtok(value, ":");
    if (str_alloc_and_copy(&(host->name), aux) < 0) {
        return -1;
    }
    aux = strtok(NULL, ":");
    if (aux == NULL) {
        host->port = 80;
    } else {
        host->port = atoi(aux);
        if (host->port < 1) {
            return -1;
        }
    }
    return 0;
}

