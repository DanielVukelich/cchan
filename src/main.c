#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <util/files.h>
#include <net/http.h>
#include <net/server.h>
#include <net/handlers.h>

int main(int argc, char **argv)
{
    int port = 80;
    HTTPServer server;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    init_handlers();
    init_HTTPServer(&server, port, 0);
    run_HTTPServer(&server);
    close_HTTPServer(&server);
    fin_handlers();
    return EXIT_SUCCESS;
}

