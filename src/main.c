#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <util/files.h>
#include <net/http.h>

int main(int argc, char **argv)
{
    int filed = open("testfiles/test_request", O_RDONLY);
    HTTPRequest* request = parse_HTTPRequest(filed);
    (void) argc;
    (void) argv;
    puts("\nmethod:");
    printf("%d\n", request->method);
    puts("\ntarget:");
    puts(request->target);
    puts("\nprotocol:");
    puts(request->protocol.name);
    puts("\nprotocol version:");
    puts(request->protocol.version);
    free_HTTPRequest(request);
    close(filed);
    return EXIT_SUCCESS;
}

