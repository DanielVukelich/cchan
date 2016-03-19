#pragma once

#define MAX_HANDLERS 32
#define MAX_HANDLER_URI_LENGTH 128 /* this only applies to the first chunk
of text between the two first slashes */

#include "http.h"

typedef int (*HandlerFunc) (HTTP_Request *request, int client_sock);

int indexHandler(HTTP_Request *request, int client_sock);
int staticHandler(HTTP_Request *request, int client_sock);

typedef struct {
    HandlerFunc handler;
    char *name;
} RequestHandler;

extern RequestHandler requestHandlersTable[MAX_HANDLERS];
extern int n_handlers;

HandlerFunc get_HandlerFunc(char str[]);

void init_handlers();
void fin_handlers();

void send_HTTP_startline(int filed, int code);
void send_HTTP_finheaders(int filed);
void send_HTTP_endmsg(int filed);

