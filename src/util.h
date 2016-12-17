// util.h
// DidiWiki - a small lightweight wiki engine
// Copyright 2004 Matthew Allum <mallum@o-hand.com> GPL-2+

char *util_mprintf(const char *format, ...);
int util_ascii_to_hex(int c);
void util_dehttpize(char *z);
char *util_extract_token(char *input, char **leftOver);
char *util_htmlize(const char *in, int n);
char *util_httpize(const char* url);
