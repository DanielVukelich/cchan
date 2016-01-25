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
#define MAX_BODY_LEN 65536

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
#define AUTOST_SL_METHOD  1 /* method of request , START HERE*/
#define AUTOST_SL_SP1     2 /* whitespace between method and target */
#define AUTOST_SL_TARGET  3 /* target */
#define AUTOST_SL_TARGETS 5 /* target's first slash */
#define AUTOST_SL_SP2     6 /* whitespace between target and protocol */
#define AUTOST_SL_PROTOCN 7 /* protocol name */
#define AUTOST_SL_PROTOCS 8 /* slash between protocol name and protocol version */
#define AUTOST_SL_PROTOCV 9 /* protocol version, FINISH HERE */

/* STATES IN HEADER */
#define AUTOST_HE_FN     1 /* field name, START HERE */
#define AUTOST_HE_COLON  2 /* colon between field name and value */
#define AUTOST_HE_OWS1   3 /* optional whitespaces before field value */
#define AUTOST_HE_FV     4 /* header field value */
#define AUTOST_HE_FVSP   5 /* single space between field values, FINISH HERE if CRLF */

/* error macros */
#define parseerror(x) puts("Parsing error: "x)

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
    char buffer[BLOCK_SIZE], thischar, nextchar;
    char method[MAX_METHOD_LEN];
    char body[MAX_BODY_LEN];
    char protocol[MAX_PROTOCOL_LEN];
    char target[MAX_TARGET_LEN];
    char header_fieldname[MAX_HEADER_FN_LEN];
    char header_fieldvalue[MAX_HEADER_FV_LEN];
    int n_chars_read = 0, /* because this is how you handle POSIX files */
        done = 0, /* set to 1 when EOF is reached */
        i = 0, /* counter when reading large blocks of read data */
        j = 0, /* counter when writing in a given buffer */
        nline = 1, /* for debugging info */
        ncol = 1, /* same */
        bodylen = 0; /* length of body */
    int first_level_state = AUTOST_START_LINE,
        second_level_state = AUTOST_SL_METHOD;
    /* buffers */
    do {
        n_chars_read = read(filed, buffer, BLOCK_SIZE);
        if (n_chars_read < BLOCK_SIZE) { /* reading last block of file */
            done = 1;
        }
        for (i = 0; i < n_chars_read - 1; ++i) {
            thischar = buffer[i];
            nextchar = buffer[i + 1];
            switch (first_level_state) {
                case AUTOST_START_LINE: /* sub automaton */
                    switch (second_level_state) {
                        case AUTOST_SL_METHOD:
                            method[j++] = thischar;
                            if (is_http_tchar(nextchar)) {
                                ;
                            } else if (nextchar == ' ') {
                                method[j] = '\0'; /* finish writing method */
                                j = 0;
                                second_level_state = AUTOST_SL_SP1;
                            } else {
                                puts("Parsing method.");
                                parseerror("expected token character or SP.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_SP1:
                            if (nextchar == '/') { /* target must start with / */
                                second_level_state = AUTOST_SL_TARGETS;
                            } else {
                                puts("Parsing startline first space.");
                                parseerror("expected slash.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break; 

                        case AUTOST_SL_TARGETS: /* target's first slash */
                            target[j++] = thischar;
                            if (nextchar == ' ') {
                                target[j] = '\0';
                                j = 0;
                                second_level_state = AUTOST_SL_SP2;
                            } else if ( is_http_tchar(nextchar) ) {
                                second_level_state = AUTOST_SL_TARGET;
                            } else {
                                puts("Parsing target's first slash.");
                                parseerror("expected either token character or SP.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_TARGET:
                            target[j++] = thischar;
                            if (isgraph(nextchar)) {
                                ;
                            } else if (nextchar == ' ') {
                                target[j] = '\0';
                                j = 0;
                                second_level_state = AUTOST_SL_SP2;
                            } else {
                                puts("Parsing target past the first slash.");
                                parseerror("expected VCHAR or SP.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_SP2:
                            if (is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_SL_PROTOCN;
                            } else {
                                puts("Parsing startline's second space.");
                                parseerror("expected token character.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_PROTOCN:
                            protocol[j++] = thischar;
                            if (is_http_tchar(nextchar)) {
                                ;
                            } else if (nextchar == '/') {
                                second_level_state = AUTOST_SL_PROTOCS;
                            } else {
                                puts("Parsing protocol name.");
                                parseerror("expected slash or token character.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_PROTOCS:
                            protocol[j++] = thischar;
                            if(is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_SL_PROTOCV;
                            } else {
                                puts("Parsing protocol separating slash.");
                                parseerror("expected token character.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        case AUTOST_SL_PROTOCV:
                            protocol[j++] = thischar;
                            if (is_http_tchar(nextchar)) {
                                ;
                            } else if (nextchar == '\r'){
                                protocol[j] = '\0';
                                j = 0;
                                first_level_state = AUTOST_CR_STARTL;
                            } else {
                                puts("Parsing protocol version.");
                                parseerror("expected token character or CR");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;

                        default: /* should not happen */
                            break;
                    };
                    break;

                case AUTOST_ERR_BADREQ:
                    printf("Line %d, column %d\n", nline, ncol);
                    point_error_in_line(buffer, i);
                    if (isgraph(thischar) ) {
                        printf("Current character: '%c', ", thischar);
                    } else {
                        printf("Current character: 0x%02x, ", thischar);
                    }
                    if (isgraph(nextchar) ){
                        printf("next character: '%c'\n", nextchar);
                    } else {
                        printf("next character: 0x%02x\n", nextchar);
                    }
                    free_HTTPRequest(request);
                    return NULL;

                case AUTOST_ERR_OTHER:
                    free_HTTPRequest(request);
                    return NULL;

                case AUTOST_CR_STARTL: /* expect a linefeed */
                    if (nextchar == '\n') {
                        first_level_state = AUTOST_LF_STARTL;
                    } else {
                        parseerror("expected LF.");
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
                        parseerror("expected token character or CR");
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER: /* sub automaton */
                    switch (second_level_state) {
                        case AUTOST_HE_FN:
                            header_fieldname[j++] = thischar;
                            if (nextchar == ':') {
                                header_fieldname[j] = '\0';
                                j = 0;
                                second_level_state = AUTOST_HE_COLON;
                            } else if (is_http_tchar(nextchar)) {
                                ;
                            } else {
                                parseerror("expected token character or ':'.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_COLON:
                            if (isspace(nextchar)) {
                                second_level_state = AUTOST_HE_OWS1;
                            } else if (is_http_tchar(nextchar)) {
                                second_level_state = AUTOST_HE_FV;
                            } else {
                                parseerror("expected token character or OWS.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_OWS1:
                            if (isgraph(nextchar)) {
                                second_level_state = AUTOST_HE_FV;
                            } else if (isspace(nextchar)) {
                                ; /* do nothing */
                            } else {
                                parseerror("expected VCHAR or OWS.");
                                first_level_state = AUTOST_ERR_BADREQ;
                            }
                            break;
                        case AUTOST_HE_FV:
                            header_fieldvalue[j++] = thischar;
                            if (isgraph(nextchar)) {
                                ;
                            } else if (isspace(nextchar)) {
                                header_fieldvalue[j] = '\0';
                                j = 0;
                                second_level_state = AUTOST_HE_FVSP;
                            } else {
                                parseerror("expected VCHAR or SP/HTAB.");
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
                                parseerror("expected VCHAR, CR or SP/HTAB.");
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
                        parseerror("expected token character or CR.");
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER_CR2: /* expect a line feed */
                    if (nextchar == '\n') {
                        first_level_state = AUTOST_HEADER_LF2;
                    } else {
                        parseerror("expected LF.");
                        first_level_state = AUTOST_ERR_BADREQ;
                    }
                    break;

                case AUTOST_HEADER_LF2: /* expect anything */
                    first_level_state = AUTOST_BODY;
                    break;

                case AUTOST_BODY: /* just accept tbqh fam */
                    body[j++] = thischar;
                    ++ bodylen;
                    break;

                default: /* should not happen, but the compiler will whine at me */
                    break;
            }; /* switch(first_state_level) */
            /* update debug info */
            ++ncol;
            if (thischar == '\n') {
                ncol = 1;
                ++ nline;
            }
        } /* reading block "for" loop */
    } while (! done);
    /* --- write collected info to request --- */
    puts(method);
    puts(target);
    (void) header_fieldname;
    (void) header_fieldvalue;
    puts(protocol);
    (void) body;
    return request;
}

