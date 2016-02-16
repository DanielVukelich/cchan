#pragma once

#include "headers.h"

#include <stdlib.h>
#include <netinet/in.h>

typedef struct {
    int socket;
    struct sockaddr_in info;
} HTTPClient;

typedef struct {
    HTTPClient client;
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
    http_ConnectionType connection;
    http_UserAgent user_agent;
    http_ProductToken upgrade;
    char *referer;
    char *origin;
    char *from;
    size_t content_length;
    /* request body */
    char* body;
} HTTPRequest;

/* 
 * Create an empty HTTP request with empty fields
 * */
HTTPRequest *new_HTTPRequest();
/* 
 * Return NULL on incorrect request
 * */
HTTPRequest *parse_HTTPRequest(HTTPRequest *request);

void free_HTTPRequest(HTTPRequest *request);

/* return 1 if header was standard, 0 otherwise*/
int parse_header(HTTPRequest *request, char line[]);

int parse_HTTPRequest_startline(HTTPRequest *request, char line[]);

int parse_HTTPRequest_protocol(HTTPRequest *request, char str[]);

