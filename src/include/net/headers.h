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
} HTTP_Method;

/* define enums for different HTTP headers*/

typedef enum {
    CONN_DEFAULT,
    KEEP_ALIVE
} HTTP_ConnectionType;


/* accept fields */
typedef struct {
    char *name;
    float qvalue;
} HTTP_AcceptField;

/* other fields */
typedef struct {
    char* name;
    char* value;
} HTTP_Cookie;

/* misc structures and enumerations */
typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} HTTP_Date;

typedef struct {
    short port;
    char* name;
} HTTP_Host;

typedef struct {
    char *name;
} HTTP_Authorization;

typedef enum {
    BASIC
} HTTP_auth_type;

typedef struct {
    char *type;
    char *subtype;
} HTTP_MediaType;

typedef struct {
    char *name;
    char *version;
} HTTP_ProductToken;

typedef struct {
    HTTP_ProductToken *products;
    int nproducts;
    char **comments;
    int ncomments;
} HTTP_UserAgent;

typedef struct {
    char *primary_tag;
    char **subtags;
    int n_subtags;
} HTTP_LanguageToken;

/* parse different HTTP header types, return 0 on success, -1 otherwise */
int parse_HTTP_Host(HTTP_Host *host, char value[]);
int parse_HTTP_LanguageToken(HTTP_LanguageToken *token, char value[]);
int parse_HTTP_Date(HTTP_Date *date, char value[]);
int parse_HTTP_ConnectionType(HTTP_ConnectionType *connection, char value[]);
int parse_HTTP_UserAgent(HTTP_UserAgent *agent, char value[]);
int parse_HTTP_ProductToken(HTTP_ProductToken *token, char value[]);

