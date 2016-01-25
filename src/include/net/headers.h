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
} httpheader_Connection;


/* accept fields */
typedef struct {
    char *name;
    float qvalue;
} httpheader_Accept_field;

/* other fields */
typedef struct {
    char* name;
    char* value;
} httpheader_Cookie;

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
} httpheader_UserAgent;

typedef struct {
    char *primary_tag;
    char *subtag;
    int n_subtags;
} http_LanguageToken;

