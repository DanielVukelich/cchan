#include <net/http.h>
#include <net/headers.h>
#include <util/http.h>
#include <util/strings.h>

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
#define methoderror(x,y) puts(x" method: "); puts(y); free(request); return NULL

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

HTTPRequest *parse_HTTPRequest(int filed)
{
    HTTPRequest* request = malloc(sizeof(HTTPRequest));
    char buffer[BLOCK_SIZE];
    char method[MAX_METHOD_LEN];
    char body[MAX_BODY_LEN];
    char protocol[MAX_PROTOCOL_LEN];
    char target[MAX_TARGET_LEN];
    char header_fieldname[MAX_HEADER_FN_LEN];
    char header_fieldvalue[MAX_HEADER_FV_LEN];
    int n_chars_read = 0, /* because this is how you handle POSIX files */
        done = 0, /* set to 1 when EOF is reached */
        i = 0, /* counter when reading large blocks of read data */
        nline = 1, /* for debugging info */

    /* ----- PARSING ----- */
    do {
        n_chars_read = read(filed, buffer, BLOCK_SIZE);
        if (n_chars_read < BLOCK_SIZE) { /* reading last block of file */
            done = 1;
        }
        for (i = 0; i < n_chars_read - 1; ++i) {
        } /* reading block "for" loop */
    } while (! done);
    /* ----- END PARSING ----- */

    /* parse last header */
    if (! parse_header(request, header_fieldname, header_fieldvalue)) {
        /* TODO: save header */
    } else {
        ; /* nothing */
    }
    request->method = get_http_method(method);
    if (request->method == INVALID_METHOD) {
        methoderror("Invalid", method);
    }
    /* --- TODO: check if request is valid --- */
    /* --- write collected info to request --- */
    (void) body;
    parse_http_ProductToken(&(request->protocol), protocol);
    str_alloc_and_copy(&(request->target), target);
    return request;
}


/* return 1 if header was standard, 0 if it was custom*/
int parse_header(HTTPRequest *req, char name[], char value[])
{
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
    } else if (strcasecmp(name, "content-length")) {
        req->content_length = atoi(value);
    } else {
        return 1;
    }
    return 0;
}

int parse_HTTPRequest_startline(HTTPRequest *req, int filed, char *buffer[])
{
    int done = 0, parsed_method = 0, parsed_target = 0, parsed_protocol = 0,
        i = 0, cursor = 0, read_chars, ret_val;
    char method_str = malloc(MAX_METHOD_LEN), c;

    /* alloc buffer */
    *buffer = malloc(BLOCK_SIZE);
    if (*buffer == NULL) {
        return -1;
    }

    while (! done) {

        /* from file to buffer */
        read_chars = read(files, *buffer, BLOCK_SIZE);
        if (read_chars < 0) {
            puts(__FILE__);
            puts("Got negative read_chars value.");
            exit(EXIT_FAILURE);
        } else if (read_chars < BLOCK_SIZE) {
            done = 1;
            buffer[read_chars] = '\0';
        }

    }

    free (method_str);
    return cursor;
}

