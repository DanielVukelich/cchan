#pragma once

#define is_http_tchar(x) (isalnum(x) || (x) == '!'\
        || (x) == '#' || (x) == '\''\
        || (x) == '$' || (x) == '*' \
        || (x) == '%' || (x) == '+' \
        || (x) == '&' || (x) == '-' \
        || (x) == '.' || (x) == '^' \
        || (x) == '_' || (x) == '`' \
        || (x) == '|' || (x) == '~')

#define is_http_ws(x) ((x) == ' ' || (x) == '\t')

/* print the line and show where the error is*/
void point_error_in_line(char buffer[], int currentpos);

