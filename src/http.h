// http.h
// DidiWiki - a small lightweight wiki engine
// Copyright 2004 Matthew Allum <mallum@o-hand.com> GPL-2+

// Ports to try listening on, starting at PORTSTART
#define PORTSTART 8000
#define PORTEND 8088
// Maximum number of child processes before things slow down
#define MAX_PARALLEL 5
#ifndef WNOHANG
#define WNOHANG 1
#endif
typedef struct HttpResponse HttpResponse;
typedef struct HttpRequest HttpRequest;
typedef struct HttpRequestParam HttpRequestParam;
HttpRequest *http_server(void);
HttpRequest *http_request_new(void);
char *http_request_param_get(HttpRequest *req, char *key);
char *http_request_get_uri(HttpRequest *req);
char *http_request_get_path_info(HttpRequest *req);
char *http_request_get_query_string(HttpRequest *req);
HttpResponse *http_response_new(HttpRequest *req);
void http_response_printf(HttpResponse *res, const char *format, ...);
void http_response_printf_alloc_buffer(HttpResponse *res, int bytes);
void http_response_set_content_type(HttpResponse *res, char *type);
void http_response_set_status(HttpResponse *res, int status_code, char *status_desc);
void http_response_set_data(HttpResponse *res, void *data, int data_len);
void http_response_append_header(HttpResponse *res, char *header);
void http_response_send_headers(HttpResponse *res);
void http_response_send(HttpResponse *res);
void sigint(int sig);
void sigterm(int sig);
