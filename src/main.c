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
    HTTP_Server server;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    init_handlers();
    init_HTTP_Server(&server, port, 0);
    run_HTTP_Server(&server);
    close_HTTP_Server(&server);
    fin_handlers();
    return EXIT_SUCCESS;
}

