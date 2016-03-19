#include <net/server.h>
#include <net/http.h>
#include <net/handlers.h>
#include <net/static.h>
#include <util/strings.h>

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

#define N_THREADS 3

void init_HTTPServer(HTTPServer *server, short port, int flags)
{
    int status;
    (void) flags;
    /* misc */
    server->flags = flags;
    /* init info structure */
    memset(&(server->info), 0, sizeof(server->info));
    server->info.sin_family = AF_INET;
    server->info.sin_addr.s_addr = htonl(INADDR_ANY);
    server->info.sin_port = htons(port);
    /* create socket */
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    /* bind socket info */
    status = bind(server->socket,
            (struct sockaddr *) &(server->info),
            sizeof(struct sockaddr));
    if (status < 0) {
        puts("Error binding port to socket");
    }
}

void run_HTTPServer(HTTPServer *server)
{
    /* WARNING: messy code */
    /* NOTE: multithreading is not implemented yet */
    int resp;
    socklen_t socket_len = sizeof(struct sockaddr_in);
    HTTP_Request *request;

    listen(server->socket, 1);

    do {
        request = new_HTTP_Request();
        request->client.socket = accept(server->socket,
                (struct sockaddr *) &(request->client.info), &socket_len);
        /* handle */
        request = parse_HTTP_Request(request);
        if (request == NULL) {
            puts("Error with request");
            send_static(request->client.socket, "/static/errors/400.html");
            resp = 400;
            continue;
        } else {
            resp = handle_HTTP_Request(server, request);
        }
        puts("Got request");
        printf("Returned %d\n", resp);
        /* done */
        free_HTTP_Request(request);
    } while (request->client.socket >= 0);
}

int handle_HTTP_Request(HTTPServer* server, HTTP_Request *request)
{
    char *aux = NULL;
    HandlerFunc handler;
    int resp;

    (void) server;
    /* get first URI string */
    /* create a copy of target because strtok is shit tbqh */
    str_alloc_and_copy(&aux, request->target);
    aux = strtok(aux, "/");
    puts(request->target);
    if (aux == NULL) { /* received "/" */
        handler = get_HandlerFunc("");
    } else {
        puts("handler string:");
        puts(aux);
        handler = get_HandlerFunc(aux);
    }
    if (handler == NULL) {
        /* send 404 */
        send_HTTP_startline(request->client.socket, 404);
        send_HTTP_finheaders(request->client.socket);
        send_static(request->client.socket, "/static/errors/404.html");
        resp = 404;
    } else {
        resp = handler(request, request->client.socket);
    }
    close(request->client.socket);
    return resp;
}

void close_HTTPServer(HTTPServer *server)
{
    close(server->socket);
}

