#include <net/http.h>
#include <net/headers.h>
#include <util/files.h>

HTTPRequest *parse_request(int filed)
{
    char *line;
    HTTPRequest* request = malloc(sizeof(HTTPRequest));
    get_nextline(filed, &line);
    return request;
};

void free_request(HTTPRequest *request)
{
    free(request);
};

