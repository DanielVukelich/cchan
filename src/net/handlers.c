#include <net/handlers.h>
#include <net/static.h>
#include <util/strings.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

HTTP_RequestHandler requestHandlersTable[];
int n_handlers;

void init_handlers()
{
    /* TODO: function to add handler to table */
    n_handlers = 2;
    requestHandlersTable[0].function = indexHandler;
    requestHandlersTable[1].function = staticHandler;
    str_alloc_and_copy(&(requestHandlersTable[0].name), "");
    str_alloc_and_copy(&(requestHandlersTable[1].name), "static");
}

void fin_handlers()
{
    int i;
    for (i = 0; i < n_handlers; ++i) {
        free(requestHandlersTable[i].name);
    }
}


void indexHandler(HTTP_Request *request, HTTP_Response *response)
{
    static char index_location[] = "static/index.html";
    (void) request;
    response->status_code = 200;
    str_alloc_and_copy(&(response->file_location), index_location);
}

void staticHandler(HTTP_Request *request, HTTP_Response *response)
{
    /* "+ 1" is to remove the initial slash*/
    str_alloc_and_copy(&(response->file_location), request->target + 1);
    response->status_code = check_file_availability(response->file_location);
}

void send_HTTP_startline(int filed, int code)
{
    /* TODO: this is just here for testing */
    char *startline;
    startline = malloc(sizeof("HTTP/1.1 XXX ") + 16);
    sprintf(startline, "HTTP/1.1 %3d %s", code, "OK");
    write(filed, startline, strlen(startline));
}

void send_HTTP_finheaders(int filed)
{
    static char crlf[] = "\r\n\r\n";
    write(filed, crlf, 4);
}

void send_HTTP_endmsg(int filed)
{
    static char crlf[] = "\r\n";
    write(filed, crlf, 2);
}

HTTP_HandlerFunction get_HTTP_Handler_from_name(char name[])
{
    int i;
    for (i = 0; i < n_handlers; ++i) {
        if (strcmp(name, requestHandlersTable[i].name) == 0) {
            return requestHandlersTable[i].function;
        }
    }
    return NULL;
}

char* parse_HTTP_Handler_from_URI(char uri[])
{
    int second_slash_position = -1;
    int i;
    int uri_has_initial_slash = uri[0] == '/';
    int handler_name_length;
    char *handler_name = NULL;

    /* handle "/" case */
    if (uri[0] == '/' && uri[1] == '\0') {
        handler_name = calloc(1, sizeof(char));
        return handler_name;
    }

    /* ignore initial slash if present */
    if (uri_has_initial_slash) {
        i = 1;
    } else {
        i = 0;
    }

    while(uri[i++] != '\0') {
        if (uri[i] == '/') {
            second_slash_position = i;
            break;
        }
    }

    /* second slash was not found, and we reached the end of the string */
    if (second_slash_position < 0) {
        second_slash_position = i - 1;
    }

    /* determine string size */
    if (uri_has_initial_slash) {
        handler_name_length = second_slash_position;
    } else {
        handler_name_length = second_slash_position + 1;
    }

    handler_name = malloc(sizeof(char) * handler_name_length);
    
    if (uri_has_initial_slash) {
        strncpy(handler_name, uri + 1, handler_name_length - 1);
    } else {
        strncpy(handler_name, uri, handler_name_length - 1);
    }
    handler_name[handler_name_length - 1] = 0;

    return handler_name;

}

HTTP_HandlerFunction get_HTTP_Handler_from_HTTP_Request(HTTP_Request *request)
{
    char *handler_name;
    HTTP_HandlerFunction handler_function;

    handler_name = parse_HTTP_Handler_from_URI(request->target);
    handler_function =  get_HTTP_Handler_from_name(handler_name);
    free(handler_name);
    return handler_function;
}

void serve_HTTP_Response(HTTP_Response *response)
{
    /* send first line */
    send_HTTP_startline(response->client.socket, response->status_code);
    /* send headers*/
    send_HTTP_finheaders(response->client.socket);
    if (response->status_code == 404) {
        send_static(response->client.socket, "static/errors/404.html");
        goto end_response;
    }

    /* serve content */
    if (response->serving_type == SERVE_STATIC) {
        send_static(response->client.socket, response->file_location);
    } else {
        /* TODO: implement templates */
        return;
    }

end_response:
    send_HTTP_endmsg(response->client.socket);
}

