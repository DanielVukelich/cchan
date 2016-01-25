#include <net/http.h>
#include <net/headers.h>
#include <util/http.h>

#include <stdlib.h>
#include <string.h>
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

/* automaton states */
/* GENERAL STATES */
#define AUTOST_START_LINE 0 /* <------------- START */
#define AUTOST_ERR_BADREQ 1 /* bad request, found error in formatting */
#define AUTOST_ERR_OTHER  2 /* could be anything */
#define AUTOST_CR_STARTL  3 /* first carry return after start line */
#define AUTOST_LF_STARTL  4 /* first line feed after start line */
#define AUTOST_HEADER     5
#define AUTOST_HEADER_CR  6 /* carry return and line feeds after headers */
#define AUTOST_HEADER_LF  7
#define AUTOST_HEADER_CR2 8 /* carry return and line feed before body */
#define AUTOST_HEADER_LF2 9
#define AUTOST_BODY      10 /* --------------> END*/

/* STATES IN START LINE */
#define AUTOST_SL_METHOD 1 /* method of request , START HERE*/
#define AUTOST_SL_SP1    2 /* whitespace between method and target */
#define AUTOST_SL_TARGET 3 /* target */
#define AUTOST_SL_SP2    4 /* whitespace between target and protocol */
#define AUTOST_SL_PROTOC 5 /* protocol version, FINISH HERE */

/* STATES IN HEADER */
#define AUTOST_HE_FN     1 /* field name, START HERE */
#define AUTOST_HE_COLON  2 /* colon between field name and value */
#define AUTOST_HE_OWS1   3 /* optional whitespaces before field value */
#define AUTOST_HE_FV     4 /* header field value */
#define AUTOST_HE_FVSP   5 /* single space between field values, FINISH HERE if CRLF */

HTTPRequest *new_HTTPRequest()
{
    HTTPRequest *req = malloc(sizeof(HTTPRequest));
    req->method = INVALID_METHOD;
    req->protocol.name = NULL;
    req->protocol.version = NULL;
    req->target = NULL;
    req->host.port = 0;
    req->host.name = NULL;
    req->accept_language.primary_tag = NULL;
    req->accept_language.subtag = NULL;
    req->accept_language.n_subtags = 0;
    req->accept_date.day = 0;
    req->accept_date.month = 0;
    req->accept_date.year = 0;
    req->accept_date.hour = 0;
    req->accept_date.minute = 0;
    req->accept_date.second = 0;
    req->connection = CONN_DEFAULT;
    req->user_agent.products = NULL;
    req->user_agent.nproducts = 0;
    req->user_agent.comments = NULL;
    req->user_agent.ncomments = 0;
    req->upgrade.name = NULL;
    req->upgrade.version = NULL;
    req->referer = NULL;
    req->origin = NULL;
    req->from = NULL;
    req->content_length = 0;
    req->body = NULL;
    return req;
}

void free_HTTPRequest(HTTPRequest *req)
{
    /* TODO: free everything if non-NULL */
    free(req);
}

HTTPRequest *parse_HTTPRequest(int filed)
{
    HTTPRequest* request = malloc(sizeof(HTTPRequest));
    char line[BLOCK_SIZE], thischar, nextchar;
    char method[MAX_METHOD_LEN];
    char target[MAX_TARGET_LEN];
    int n_chars_read = 0, done = 0, i = 0, j = 0;
    int first_level_state = AUTOST_START_LINE,
        second_level_state = AUTOST_SL_METHOD;
    /* buffers */
    do {
        n_chars_read = read(filed, line, BLOCK_SIZE);
        if (n_chars_read < BLOCK_SIZE) { /* reading last block of file */
            done = 1;
        }
        for (i = 0; i < n_chars_read - 1; ++i) {
            thischar = line[i];
            nextchar = line[i + 1];
            switch (first_level_state) {
                case AUTOST_START_LINE: /* sub automaton */
                    switch (second_level_state) {
                        case AUTOST_SL_METHOD:
                            if (is_http_tchar(nextchar)) {
                                ; /* TODO: copy to method buffer*/
                            } else if (nextchar == ' ') {
                                second_level_state = AUTOST_SL_SP1;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_SL_SP1:
                            if (is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_SL_TARGET;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_SL_TARGET:
                            if (is_http_tchar(nextchar)) {
                                ; /* copy to target buffer */
                            } else if (nextchar == ' ') {
                                second_level_state = AUTOST_SL_SP2;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_SL_SP2:
                            if (is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_SL_PROTOC;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_SL_PROTOC:
                            if (is_http_tchar(nextchar)) {
                                ; /* save to protocol buffer */
                            } else if (nextchar == '\r'){
                                first_level_state = AUTOST_CR_STARTL;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        default: /* should not happen */
                            break;
                    };
                    break;

                case AUTOST_ERR_BADREQ:
                    /* TODO: return error */
                    return NULL;

                case AUTOST_ERR_OTHER:
                    /* TODO: return error */
                    return NULL;

                case AUTOST_CR_STARTL: /* expect a linefeed */
                    if (nextchar == '\n') {
                        first_level_state = AUTOST_LF_STARTL;
                    } else {
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_LF_STARTL: /* expect either a header or a carry return */
                    if (is_http_tchar(nextchar) ) { /* token character, header */
                        first_level_state = AUTOST_HEADER;
                        second_level_state = AUTOST_HE_FN;
                    } else if (nextchar == '\r') {  /* no headers, second empty line */
                        first_level_state = AUTOST_HEADER_CR2;
                    } else {
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER: /* sub automaton */
                    switch (second_level_state) {
                        case AUTOST_HE_FN:
                            if (nextchar == ':') {
                                second_level_state = AUTOST_HE_COLON;
                            } else if (is_http_tchar(nextchar)) {
                                ; /* TODO: copy to header buffer */
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_COLON:
                            if (isspace(nextchar)) {
                                second_level_state = AUTOST_HE_OWS1;
                            } else if (is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_HE_FV;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_OWS1:
                            if (isgraph(nextchar)) {
                                second_level_state = AUTOST_HE_FV;
                            } else if (isspace(nextchar)) {
                                ; /* do nothing */
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_FV:
                            if (isgraph(nextchar)) {
                                ; /* TODO: copy to field value buffer */
                            } else if (isspace(nextchar)) {
                                second_level_state = AUTOST_HE_FVSP;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_FVSP:
                            if (isgraph(nextchar)) {
                                second_level_state = AUTOST_HE_FV;
                                /* TODO: create new field value buffer */
                            } else if (isspace(nextchar)){
                                ; /* do nothing */
                            } else if (nextchar == '\r') {
                                first_level_state = AUTOST_HEADER_CR;
                            } else {
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        default: /* should not happen */
                            break;
                    };
                    break;

                case AUTOST_HEADER_CR: /* expect a linefeed */
                    if (nextchar == '\n' ) {
                        first_level_state = AUTOST_HEADER_LF;
                    } else {
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER_LF: /* expect either a header or a carry return */
                    if (is_http_tchar(nextchar) ) { /* token, therefore field name */
                        first_level_state = AUTOST_HEADER;
                        second_level_state = AUTOST_HE_FN;
                    } else if (nextchar == '\r') { /* second empty line */
                        first_level_state = AUTOST_HEADER_CR2;
                    } else {
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER_CR2: /* expect a line feed */
                    if (nextchar == '\n') {
                        first_level_state = AUTOST_HEADER_LF2;
                    } else {
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER_LF2: /* expect anything */
                    first_level_state = AUTOST_BODY;
                    break;

                case AUTOST_BODY: /* just accept tbqh fam */
                    ; /* TODO: write to body buffer */
                    break;

                default: /* should not happen, but the compiler will whine at me */
                    break;
            };
        }
    } while (! done);
    return request;
}

