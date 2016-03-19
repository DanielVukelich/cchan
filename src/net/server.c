#include <net/server.h>
#include <net/http.h>
#include <net/handlers.h>
#include <net/static.h>
#include <util/strings.h>
#include <util/http.h>

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
    HandlerFunc handler;
    char *handler_name;
    int response_code;

    (void) server;
    /* get first URI string */
    puts("request target:");
    puts(request->target);
    handler_name = get_handler_name_from_URI(request->target);
    puts("handler string:");
    puts(handler_name);
    handler = get_HandlerFunc(handler_name);
    if (handler == NULL) {
        /* send 404 */
        send_HTTP_startline(request->client.socket, 404);
        send_HTTP_finheaders(request->client.socket);
        send_static(request->client.socket, "/static/errors/404.html");
        response_code = 404;
    } else {
        response_code = handler(request, request->client.socket);
    }
    free(handler_name);
    close(request->client.socket);
    return response_code;
}

void close_HTTPServer(HTTPServer *server)
{
    close(server->socket);
}

