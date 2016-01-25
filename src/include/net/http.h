#pragma once

#define HTTP_BUFFER_SIZE 2048

#include "headers.h"

#include <stdlib.h>

typedef struct {
    http_method method;
    http_ProductToken protocol;
    char* target;
    /* headers */
    http_Host host;
    /* content negociation */
    /*httpheader_Accept accept;
    httpheader_Accept accept_charset;
    httpheader_Accept accept_encoding;*/
    http_LanguageToken accept_language;
    http_Date         accept_date;
    /* other */
    httpheader_Connection connection;
    httpheader_UserAgent user_agent;
    http_ProductToken upgrade;
    char *referer;
    char *origin;
    char *from;
    size_t content_length;
    /* request body */
    char* body;
} HTTPRequest;

typedef struct {
    int socket_fd;
    char buffer[HTTP_BUFFER_SIZE];
} HTTPServerSocket;

/* 
 * Create an empty HTTP request with empty fields
 * */
HTTPRequest *new_HTTPRequest();
/* 
 * Return NULL on incorrect request
 * */
HTTPRequest *parse_HTTPRequest(int filed);

void free_HTTPRequest(HTTPRequest *request);

