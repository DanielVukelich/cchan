#pragma once

/* request methods */
typedef enum {
    INVALID_METHOD,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH
} http_method;

/* define enums for different HTTP headers*/

typedef enum {
    CONN_DEFAULT,
    KEEP_ALIVE
} http_ConnectionType;


/* accept fields */
typedef struct {
    char *name;
    float qvalue;
} http_Accept_field;

/* other fields */
typedef struct {
    char* name;
    char* value;
} http_Cookie;

/* misc structures and enumerations */
typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} http_Date;

typedef struct {
    short port;
    char* name;
} http_Host;

typedef struct {
    char *name;
} http_Authorization;

typedef enum {
    BASIC
} http_auth_type;

typedef struct {
    char *type;
    char *subtype;
} http_MediaType;

typedef struct {
    char *name;
    char *version;
} http_ProductToken;

typedef struct {
    http_ProductToken *products;
    int nproducts;
    char **comments;
    int ncomments;
} http_UserAgent;

typedef struct {
    char *primary_tag;
    char **subtags;
    int n_subtags;
} http_LanguageToken;

/* parse different HTTP header types, return 0 on success, -1 otherwise */
int parse_http_Host(http_Host *host, char value[]);
int parse_http_LanguageToken(http_LanguageToken *token, char value[]);
int parse_http_Date(http_Date *date, char value[]);
int parse_http_ConnectionType(http_ConnectionType *connection, char value[]);
int parse_http_UserAgent(http_UserAgent *agent, char value[]);
int parse_http_ProductToken(http_ProductToken *token, char value[]);

