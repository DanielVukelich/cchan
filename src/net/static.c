#include <net/static.h>

#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 2048

void send_static(int socket, char filename[])
{
    static int fileopts = O_RDONLY;
    int filed, readbytes;
    char buffer[BUFFER_SIZE]; /* load from file to buffer, and send from
                          buffer to socket */
    
    /* filename + 1 to strip off the initial slash */
    filed = open(filename, fileopts);

    do {
        readbytes = read(filed, buffer, BUFFER_SIZE);
        if (readbytes < 0) {
            puts("Error reading from file.");
            return;
        } else {
            write(socket, buffer, readbytes);
        }
    } while (readbytes);
    close(filed);
}

int check_file_availability(char filename[])
{
    static int fileopts = O_RDONLY;
    int filed;

    filed = open(filename, fileopts);
    if (filed < 0) {
        switch(errno) {
            case EPERM:
                return 401;
            case ENOENT:
                return 404;
            default:
                return 500;
        }
    } else {
        return 200;
    }
}

