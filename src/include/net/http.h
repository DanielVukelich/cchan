#pragma once

#include "headers.h"

#include <stdlib.h>
#include <netinet/in.h>

typedef struct {
    int socket;
    struct sockaddr_in info;
} HTTP_Client;

typedef struct {
    HTTP_Client client;
    HTTP_Method method;
    HTTP_ProductToken protocol;
    char* target;
    /* headers */
    HTTP_Host host;
    /* content negociation */
    /*httpheader_Accept accept;
    httpheader_Accept accept_charset;
    httpheader_Accept accept_encoding;*/
    HTTP_AcceptLanguage accept_language;
    HTTP_Date         accept_date;
    /* other */
    HTTP_ConnectionType connection;
    HTTP_UserAgent user_agent;
    HTTP_ProductToken upgrade;
    char *referer;
    char *origin;
    char *from;
    size_t content_length;
    /* request body */
    char* body;
} HTTP_Request;

/* 
 * Create an empty HTTP request with empty fields
 * */
HTTP_Request *new_HTTP_Request();

/* 
 * Return NULL on incorrect request
 * */
HTTP_Request *parse_HTTP_Request(HTTP_Request *request);

void free_HTTP_Request(HTTP_Request *request);

/* return 1 if header was standard, 0 otherwise*/
int parse_header(HTTP_Request *request, char line[]);

int parse_HTTP_Request_startline(HTTP_Request *request, char line[]);

int parse_HTTP_Request_protocol(HTTP_Request *request, char str[]);

