#pragma once

#define MAX_HANDLERS 32
#define MAX_HANDLER_URI_LENGTH 128 /* this only applies to the first chunk
of text between the two first slashes */

#include "http.h"

typedef void (*HTTP_HandlerFunction) (HTTP_Request *request, HTTP_Response* response);

void indexHandler(HTTP_Request *request, HTTP_Response *response);
void staticHandler(HTTP_Request *request, HTTP_Response *response);

typedef struct {
    HTTP_HandlerFunction function;
    char *name;
} HTTP_RequestHandler;

extern HTTP_RequestHandler requestHandlersTable[MAX_HANDLERS];
extern int n_handlers;

void init_handlers();
void fin_handlers();

void send_HTTP_startline(int filed, int code);
void send_HTTP_finheaders(int filed);
void send_HTTP_endmsg(int filed);

char* parse_HTTP_Handler_from_URI(char uri[]);
HTTP_HandlerFunction get_HTTP_Handler_from_name(char name[]);
HTTP_HandlerFunction get_HTTP_Handler_from_HTTP_Request(HTTP_Request *request);

void serve_HTTP_Response(HTTP_Response *response);

