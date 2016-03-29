#pragma once
#include "http.h"

#include <netinet/in.h>

typedef struct {
    struct sockaddr_in info;
    int socket;
    int flags;
    short port;
} HTTP_Server;

void init_HTTP_Server(HTTP_Server *server, short port, int flags);

void run_HTTP_Server(HTTP_Server *server);

void close_HTTP_Server(HTTP_Server *server);

