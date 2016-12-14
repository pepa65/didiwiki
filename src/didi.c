#include "didi.h"

void usage()
{
	fprintf(stderr, "Usage: %s [help] [debug]\n", PACKAGE);
	exit(1);
}

int main(int argc, char **argv)
{
	HttpRequest *req=NULL;
	wiki_init();
	// reads request from stdin, or forks
	if (argc > 1 && !strcmp(argv[1],"debug")) req=http_request_new();
	if (argc > 1 && !strcmp(argv[1],"help")) usage();
	else req=http_server();
	wiki_handle_http_request(req);
	return 0;
}
