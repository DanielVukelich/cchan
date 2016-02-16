#include <net/http.h>
#include <net/headers.h>
#include <util/http.h>
#include <util/strings.h>
#include <util/files.h>

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

/* buffer sizes */
#define BLOCK_SIZE 2048
#define MAX_METHOD_LEN 256
#define MAX_TARGET_LEN 8196
#define MAX_PROTOCOL_LEN 256
#define MAX_HEADER_FV_LEN 4096
#define MAX_HEADER_FN_LEN 256
#define MAX_BODY_LEN 65536

/* error macros */
#define parseerror(x) puts("Parsing error: "x)
#define methoderror(x,y) puts(x" method: "); puts(y); free_HTTPRequest(request); return NULL

HTTPRequest *new_HTTPRequest()
{
    HTTPRequest *req = malloc(sizeof(HTTPRequest));
    memset(req, 0, sizeof(HTTPRequest));
    req->method = INVALID_METHOD;
    return req;
}

void free_HTTPRequest(HTTPRequest *req)
{
    int i;
    /* TODO: free everything if non-NULL */
    if (req->protocol.name != NULL) {
        free(req->protocol.name);
    }
    if (req->protocol.version != NULL) {
        free(req->protocol.version);
    }
    if (req->target != NULL) {
        free(req->target);
    }
    if (req->host.name != NULL) {
        free(req->host.name);
    }
    if (req->accept_language.primary_tag != NULL) {
        free(req->accept_language.primary_tag);
    }
    if (req->accept_language.subtags != NULL) {
        for (i = 0; i < req->accept_language.n_subtags; ++i) {
            free(req->accept_language.subtags[i]);
        }
        free(req->accept_language.subtags);
    }
    if (req->user_agent.products != NULL) {
        for (i = 0; i < req->user_agent.nproducts; ++i) {
            free(req->user_agent.products + i);
        }
        free(req->user_agent.products);
    }
    if (req->user_agent.comments != NULL) {
        for (i = 0; i < req->user_agent.ncomments; ++i) {
            free(req->user_agent.comments[i]);
        }
        free(req->user_agent.comments);
    }
    if (req->upgrade.name != NULL) {
        free(req->upgrade.name);
    }
    if (req->upgrade.version != NULL) {
        free(req->upgrade.version);
    }
    if (req->referer != NULL) {
        free(req->referer);
    }
    if (req->origin != NULL) {
        free(req->origin);
    }
    if (req->from != NULL) {
        free(req->from);
    }
    if (req->body != NULL) {
        free(req->body);
    }
    free(req);
}

HTTPRequest *parse_HTTPRequest(HTTPRequest *request)
{
    char *line, *extra;
    /* parse startline */
    get_nextline_blocks(request->client.socket, &line, &extra, NULL, BLOCK_SIZE);
    puts (line);
    parse_HTTPRequest_startline(request, line);
    if (request->target == NULL) {
        puts("Null target!");
    }
    return request;
}

/* return 1 if header was standard, 0 if it was custom, -1 if wrongly formatted */
int parse_header(HTTPRequest *req, char line[])
{
    char *value, *name, *strtok_buffer = malloc(strlen(line) + 1);

    name = strtok_r(line, ":", &strtok_buffer);
    value = strtok_r(NULL, "\r", &strtok_buffer);
    if (value == NULL) {
        free(strtok_buffer);
        return -1;
    }
    while (*value++ == ' ');

    /* check header name */
    /* TODO: check for headers validity */
    if (strcasecmp(name, "host") == 0){ /* host, should be first header */
        parse_http_Host(&(req->host), value);
    } else if (strcasecmp(name, "accept-language") == 0) {
        parse_http_LanguageToken(&(req->accept_language), value);
    } else if (strcasecmp(name, "accept-date") == 0) {
        parse_http_Date(&(req->accept_date), value);
    } else if (strcasecmp(name, "connection") == 0) {
        parse_http_ConnectionType(&(req->connection), value);
    } else if (strcasecmp(name, "user-agent") == 0) {
        parse_http_UserAgent(&(req->user_agent), value);
    } else if (strcasecmp(name, "upgrade") == 0) {
        parse_http_ProductToken(&(req->upgrade), value);
    } else if (strcasecmp(name, "referer") == 0) {
        str_alloc_and_copy(&(req->referer), value);
    } else if (strcasecmp(name, "origin") == 0) {
        str_alloc_and_copy(&(req->origin), value);
    } else if (strcasecmp(name, "from") == 0) {
        str_alloc_and_copy(&(req->from), value);
    } else if (strcasecmp(name, "content-length") == 0) {
        req->content_length = atoi(value);
    } else {
        free(strtok_buffer);
        return 1;
    }
    free(strtok_buffer);
    return 0;
}

int parse_HTTPRequest_startline(HTTPRequest *req, char line[])
{
    char *aux, *strtok_buffer;

    /* get method */
    aux = strtok_r(line, " ", &strtok_buffer);
    req->method = get_http_method(aux);
    /* get target */
    aux = strtok_r(NULL, " ", &strtok_buffer);
    ++aux; /* go past whitespace */
    str_alloc_and_copy(&(req->target), aux);
    /* get protocol */
    aux = strtok_r(NULL, "", &strtok_buffer);
    ++aux;
    if (parse_HTTPRequest_protocol(req, aux) < 0) {
        puts("Invalid protocol");
        return -1;
    }
    return 0;
}

int parse_HTTPRequest_protocol(HTTPRequest *req, char str[])
{
    char *strtok_buffer, *aux;
    
    /* get protocol name */
    aux = strtok_r(str, "/", &strtok_buffer);
    str_alloc_and_copy(&(req->protocol.name), aux);
    aux = strtok_r(NULL, "", &strtok_buffer);
    if (aux == NULL) {
         return -1;
    }
    str_alloc_and_copy(&(req->protocol.version), aux);
    return 0;
}

