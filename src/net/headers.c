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

int parse_http_LanguageToken(http_LanguageToken *host, char value[])
{
    /* TODO: implement */
    (void) value; (void) host;
    return 0;
}

int parse_http_Date(http_Date *date, char value[])
{
    /* TODO: implement */
    (void) value; (void) date;
    return 0;
}

int parse_http_ConnectionType(http_ConnectionType *connection, char value[])
{
    /* TODO: implement */
    (void) value; (void) connection;
    return 0;
}

int parse_http_UserAgent(http_UserAgent *agent, char value[])
{
    /* TODO: implement */
    (void) value; (void) agent;
    return 0;
}

int parse_http_ProductToken(http_ProductToken *token, char value[])
{
    char *aux;
    aux = strtok(value, "/");
    str_alloc_and_copy(&(token->name), aux);
    aux = strtok(NULL, "/");
    if (aux != NULL) {
        str_alloc_and_copy(&(token->version), aux);
    }
    return 0;
}

