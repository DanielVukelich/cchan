#include <net/static.h>

#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 2048

int send_static(int socket, char filename[])
{
    static int fileopts = O_RDONLY;
    int filed, readbytes;
    char buffer[BUFFER_SIZE]; /* load from file to buffer, and send from
                          buffer to socket */
    
    /* filename + 1 to strip off the initial slash */
    filed = open(filename + 1, fileopts);
    if (filed < 0) {
        puts("File could not be open.");
        puts(filename);
        switch(errno) {
            case EPERM:
                puts("Access denied.");
                return 401;
            case ENOENT:
                puts("File does not exist.");
                return 404;
            default:
                puts("Unsupported error.");
                return 503;
        };
    } /* if (filed < 0) */
    do {
        readbytes = read(filed, buffer, BUFFER_SIZE);
        if (readbytes < 0) {
            puts("Error reading from file.");
            return 503;
        } else {
            write(socket, buffer, readbytes);
        }
    } while (readbytes);
    close(filed);
    return 200;
}

