#pragma once
#include "http.h"

#include <netinet/in.h>

typedef struct {
    struct sockaddr_in info;
    int socket;
    int flags;
    short port;
} HTTPServer;

void init_HTTPServer(HTTPServer *server, short port, int flags);

void run_HTTPServer(HTTPServer *server);

int handle_HTTPRequest(HTTPServer *server, HTTPRequest *request);

void close_HTTPServer(HTTPServer *server);

