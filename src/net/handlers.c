#include <net/handlers.h>
#include <util/strings.h>

#include <unistd.h>
#include <string.h>

RequestHandler requestHandlersTable[];
int n_handlers;

void init_handlers()
{
    /* TODO: function to add handler to table */
    n_handlers = 1;
    requestHandlersTable[0].handler = indexHandler;
    str_alloc_and_copy(&(requestHandlersTable[0].name), "");
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

void send_http_startline(int filed, int code)
{
    /* TODO: this is just here for testing */
    static char startline[] = "HTTP/1.1 200 OK\r\n";
    (void) code;
    write(filed, startline, strlen(startline));
}

void send_http_finheaders(int filed)
{
    static char crlf[] = "\r\n";
    write(filed, crlf, 2);
}

void send_http_endmsg(int filed)
{
    static char crlf[] = "\r\n";
    write(filed, crlf, 2);
}

