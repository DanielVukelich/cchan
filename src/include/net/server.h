#pragma once
#include "http.h"

#include <netinet/in.h>

typedef struct {
    struct sockaddr_in server_info;
    int server_socket;
    int flags;
    short port;
} HTTPServer;

void init_HTTPServer(HTTPServer *server, short port, int flags);

void run_HTTPServer(HTTPServer *server);

void close_HTTPServer(HTTPServer *server);

