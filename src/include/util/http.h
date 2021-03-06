#pragma once

#define is_HTTP_tchar(x) (isalnum(x) || (x) == '!'\
        || (x) == '#' || (x) == '\''\
        || (x) == '$' || (x) == '*' \
        || (x) == '%' || (x) == '+' \
        || (x) == '&' || (x) == '-' \
        || (x) == '.' || (x) == '^' \
        || (x) == '_' || (x) == '`' \
        || (x) == '|' || (x) == '~')

#define is_HTTP_ws(x) ((x) == ' ' || (x) == '\t')
#include <net/headers.h>

/* print the line and show where the error is*/
void point_error_in_line(char buffer[], int currentpos);

/* get a method type from a string representation */
HTTP_Method get_HTTP_Method(char str[]);

/* get handler strign in URI, i.e. "handler" in /<handler>/some/resource.
 *  returns a brand new string that must be freed */
char *get_handler_name_from_URI(char uri[]);

