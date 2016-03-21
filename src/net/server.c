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

void init_HTTP_Server(HTTP_Server *server, short port, int flags)
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

void run_HTTP_Server(HTTP_Server *server)
{
    /* WARNING: messy code */
    /* NOTE: multithreading is not implemented yet */
    socklen_t socket_len = sizeof(struct sockaddr_in);
    HTTP_Request *request = new_HTTP_Request();
    HTTP_Response *response;
    HTTP_HandlerFunction http_handler;

    listen(server->socket, 1);

    do {
        /* accept connection */
        request->client.socket = accept(server->socket,
                (struct sockaddr *) &(request->client.info), &socket_len);
        /* create and parse request */
        request = parse_HTTP_Request(request);
        /* get handler for request */
        response = new_HTTP_Response();
        response->client = request->client;
        http_handler = get_HTTP_Handler_from_HTTP_Request(request);
        if (http_handler == NULL) {
            response->status_code = 404;
        } else {
            http_handler(request, response);
        }
        /* serve response */
        serve_HTTP_Response(response);
        /* done */
        close(response->client.socket);
        free_HTTP_Response(response);
        response = NULL;
    } while (request->client.socket >= 0);
    free_HTTP_Request(request);
}

void close_HTTP_Server(HTTP_Server *server)
{
    close(server->socket);
}

