// wiki.h
// DidiWiki - a small lightweight wiki engine
// Copyright 2004 Matthew Allum <mallum@o-hand.com> GPL-2+

#ifndef R_OK
#define R_OK 4
#endif

#define ULIST 0
#define OLIST 1
#define LISTS 2

typedef struct WikiPageList WikiPageList;
struct WikiPageList {
	char *name;
	time_t mtime;
};
void wiki_handle_http_request(HttpRequest *req);
void wiki_show_header(HttpResponse *res, char *page_title);
void wiki_show_footer(HttpResponse *res);
void wiki_print_data_as_html(HttpResponse *res, char *raw_page_data);
int wiki_init(void);
int asprintf(char **strp, const char *fmt, ...);
int vasprintf(char **strp, const char *fmt, va_list ap);
char *strcasestr(const char *haystack, const char *needle);
