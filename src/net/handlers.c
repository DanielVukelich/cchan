#include <net/handlers.h>
#include <net/static.h>
#include <util/strings.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

RequestHandler requestHandlersTable[];
int n_handlers;

void init_handlers()
{
    /* TODO: function to add handler to table */
    n_handlers = 2;
    requestHandlersTable[0].handler = indexHandler;
    requestHandlersTable[1].handler = staticHandler;
    str_alloc_and_copy(&(requestHandlersTable[0].name), "");
    str_alloc_and_copy(&(requestHandlersTable[1].name), "static");
}

void fin_handlers()
{
    int i;
    for (i = 0; i < n_handlers; ++i) {
        free(requestHandlersTable[0].name);
    }
}

HandlerFunc get_HandlerFunc(char str[])
{
    int i;
    for (i = 0; i < n_handlers; ++i) {
        if (strcmp(str, requestHandlersTable[i].name) == 0) {
            return requestHandlersTable[i].handler;
        }
    }
    return NULL;
}

int indexHandler(HTTPRequest *request, int client_sock)
{
    /* TODO: remove. this is just here for testing */
    static char resp[] = "<html><head></head><body><h1>Index </h1></body></html>";
    (void) request;
    send_http_startline(client_sock, 200); /* startline */
    send_http_finheaders(client_sock); /* no headers needed */
    write(client_sock, resp, sizeof(resp));
    send_http_endmsg(client_sock);
    return 200;
}

int staticHandler(HTTPRequest *request, int client_sock)
{
    int ret; /* return code */

    /* TODO: check for errors before sending startline */
    send_http_startline(client_sock, 200);
    send_http_finheaders(client_sock);
    ret = send_static(client_sock, request->target);
    send_http_endmsg(client_sock);
    return ret;
}

void send_http_startline(int filed, int code)
{
    /* TODO: this is just here for testing */
    char *startline;
    startline = malloc(sizeof("HTTP/1.1 XXX ") + 16);
    sprintf(startline, "HTTP/1.1 %3d %s", code, "OK");
    write(filed, startline, strlen(startline));
}

void send_http_finheaders(int filed)
{
    static char crlf[] = "\r\n\r\n";
    write(filed, crlf, 4);
}

void send_http_endmsg(int filed)
{
    static char crlf[] = "\r\n";
    write(filed, crlf, 2);
}

