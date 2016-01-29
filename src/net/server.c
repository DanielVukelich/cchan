#include <net/server.h>
#include <net/http.h>
#include <net/handlers.h>

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
    memset(&(server->server_info), 0, sizeof(server->server_info));
    server->server_info.sin_family = AF_INET;
    server->server_info.sin_addr.s_addr = htonl(INADDR_ANY);
    server->server_info.sin_port = htons(port);
    /* create socket */
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    /* bind socket info */
    status = bind(server->server_socket,
            (struct sockaddr *) &(server->server_info),
            sizeof(struct sockaddr));
    if (status < 0) {
        puts("Error binding port to socket");
    }
}

void run_HTTPServer(HTTPServer *server)
{
    /* WARNING: messy code */
    /* NOTE: multithreading is not implemented yet */
    /* int i; */
    struct sockaddr_in clients_info[N_THREADS];
    int client_sockets[N_THREADS];
    HandlerFunc handler;
    char *aux;
    int resp;
    socklen_t socket_len = sizeof(struct sockaddr_in);
    HTTPRequest *requests[N_THREADS] = {NULL};

    listen(server->server_socket, 1);

    do {
        client_sockets[0] = accept(server->server_socket,
                (struct sockaddr *) clients_info, &socket_len);
        /* handle */
        requests[0] = parse_HTTPRequest(client_sockets[0]);
        if (requests[0] == NULL) {
            puts("Error with request");
        }
        puts ("Got request");
        /* get first URI string */
        aux = strtok(requests[0]->target, "/");
        puts(requests[0]->target);
        if (aux == NULL) { /* received "/" */
            handler = get_HandlerFunc("");
        } else {
            puts("Other than \"\"");
            handler = get_HandlerFunc(aux);
        }
        if (handler == NULL) {
            puts("NULL handler");
        }
        resp = handler(requests[0], client_sockets[0]);
        printf("Returned %d\n", resp);
        close(client_sockets[0]);
        /* done */
        free_HTTPRequest(requests[0]);
    } while (client_sockets[0]);
}

void close_HTTPServer(HTTPServer *server)
{
    close(server->server_socket);
}
