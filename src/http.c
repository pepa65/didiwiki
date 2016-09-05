/* 
 *  DidiWiki - a small lightweight wiki engine. 
 *
 *  Parts of this http and util code based on cvstrac sources. 
 *  See http://www.cvstrac.org
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "didi.h"

struct HttpRequestParam  {
  char *key;
  char *val;
};

struct HttpRequest  
{
  HttpRequestParam *params;
  int               n_params;
  int               n_params_alloc;
  char             *uri;
  char             *path_info;
  char             *query_string;
};

struct HttpResponse 
{
  char *content_type;
  char *status_str;
  int   status;

  char *extra_headers;

  char *data;
  int   data_len;
  int   data_len_alloced;
};

/*
** Send a reply indicating that the HTTP request was malformed
*/
static void 
malformed_request(void)
{
  HttpResponse *res = NULL;

  res = http_response_new(NULL);

  http_response_set_status(res, 501, "Not Implemented");
  http_response_printf(res, "<html><body>Unrecognized HTTP Request</body></html>\n");
  http_response_send(res);

  exit(0);
}

/*
 * Add another query parameter or cookie to the parameter set.
 */
static void 
http_request_add_param(HttpRequest *req, char *key, char *value)
{
  if( req->n_params_alloc <= req->n_params )
    {
      req->n_params_alloc = req->n_params_alloc*2 + 10;
      req->params = realloc( req->params, 
			     req->n_params_alloc * sizeof(HttpRequestParam));
      if( !req->params ) exit(1); /* XXX handle better  */
    }

  req->params[req->n_params].key = key;
  req->params[req->n_params].val = value;

  req->n_params++;
}


/*
 * Add a list of query parameters or cookies to the parameter set.
 *
 * Each parameter is of the form NAME=VALUE.  Both the NAME and the
 * VALUE may be url-encoded ("+" for space, "%HH" for other special
 * characters).  But this routine assumes that NAME contains no
 * special character and therefore does not decode it.
 *
 * Parameters are separated by the "terminator" character.  Whitespace
 * before the NAME is ignored.
 *
 * The input string "z" is modified but no copies is made.  "z"
 * should not be deallocated or changed again after this routine
 * returns or it will corrupt the parameter table.
 */
static void 
http_request_parse_params (HttpRequest *req, 
			   char        *data, 
			   int          terminator)
{
  while( *data )
    {
      char *key = NULL, *val = NULL;

      while( isspace(*data) ){ data++; }
      
      key = data;

      while( *data && *data != '=' && *data != terminator )
	data++; 
      
      if( *data == '=' )
	{
	  *data = '\0';
	  data++;
	  val= data;

	  while( *data && *data != terminator ) 
	    data++; 

	  if( *data ) { *data = '\0'; data++; }
	  
	  util_dehttpize(val);

	  http_request_add_param(req, key, val);

	}
      else
	{
	  if( *data ){ *data++ = 0; }

	  http_request_add_param(req, key, NULL);
	}
    }
}


char*
http_request_param_get(HttpRequest *req, char *key)
{
  int i;

  for (i=0; i<req->n_params; i++)
    if (!strcmp(req->params[i].key, key))
      return req->params[i].val;

  return NULL;
} 

char*
http_request_get_uri(HttpRequest *req)
{
  return req->uri;
}

char*
http_request_get_path_info(HttpRequest *req)
{
  return req->path_info;
}

char*
http_request_get_query_string(HttpRequest *req)
{
  return req->query_string;
}


/*
 * This routine handles a single HTTP request which is coming in on
 * standard input ( and which replies on standard output. ). 
 * 
 */
HttpRequest*
http_request_new(void)
{
  char              *token, *content_type, *post_data, *z;
  int                i, len;
  struct sockaddr_in remoteName;
  int                size = sizeof(struct sockaddr_in);
  char               request_line[2048];     /* A single line of input. */
  HttpRequest       *req = NULL;

  req = malloc(sizeof(HttpRequest));
  memset(req, 0, sizeof(HttpRequest)); 

  if( fgets(request_line, sizeof(request_line), stdin) == NULL )
    malformed_request();

  if ((token = util_extract_token(request_line, &z)) == NULL)
    malformed_request();

  if(!(!strcmp(token,"GET") || !strcmp(token,"POST") || !strcmp(token,"HEAD")))
    malformed_request();

  putenv("GATEWAY_INTERFACE=CGI/1.0");
  putenv(util_mprintf("REQUEST_METHOD=%s",token));

  if ((token = util_extract_token(z, &z)) == NULL)
    malformed_request();

  putenv(util_mprintf("REQUEST_URI=%s", token));

  if (token)
    req->uri = strdup(token);

  /* eg. GET /blah.htm?crap HTTP/1.0 */

  for(i=0; token[i] && token[i]!='?'; i++);

  if( token[i] ) token[i++] = 0;

  putenv(util_mprintf("PATH_INFO=%s", token));

  if (token)
    req->path_info = strdup(token);

  putenv(util_mprintf("QUERY_STRING=%s", &token[i]));

  if (&token[i])
    req->query_string = strdup(&token[i]);

  if( getpeername(fileno(stdin), (struct sockaddr*)&remoteName, &size) >=0 )
    putenv(util_mprintf("REMOTE_ADDR=%s", inet_ntoa(remoteName.sin_addr)));

  /* Get all the optional fields that follow the first line.
  */
  while( fgets(request_line, sizeof(request_line), stdin) )
    {
      char *key = NULL, *val = NULL;

      key = util_extract_token(request_line, &val);

      if (key == NULL || *key == '\0')
	break;

      /* strip any spacing around key */
      while( isspace(*val) ){ val++; }
      i = strlen(val);
      while( i>0 && isspace(val[i-1]) ){ i--; } val[i] = '\0';

      /* and lower case it */
      for(i=0; key[i]; i++)
	key[i] = tolower(key[i]); 

      if(!strcmp(key,"user-agent:")) 
	{
	  putenv(util_mprintf("HTTP_USER_AGENT=%s", val));
	} 
      else if (!strcmp(key,"content-length:"))
	{
	  putenv(util_mprintf("CONTENT_LENGTH=%s", val));
	}
      else if (!strcmp(key,"referer:"))
	{
	  putenv(util_mprintf("HTTP_REFERER=%s", val));
	} 
      else if (!strcmp(key,"host:"))
	{
	  putenv(util_mprintf("HTTP_HOST=%s", val));
	}
      else if (!strcmp(key,"content-type:"))
	{
	  putenv(util_mprintf("CONTENT_TYPE=%s", val));
	}
      else if (!strcmp(key,"cookie:"))
	{
	  putenv(util_mprintf("HTTP_COOKIE=%s", val));
	}
  }

  /* Parse and store QUERY_STRING/POST/Cookie data in req object */

  if (getenv("QUERY_STRING"))
    http_request_parse_params (req, strdup((getenv("QUERY_STRING"))), '&'); 

  /* POST type data */

  content_type = getenv("CONTENT_TYPE");
  len = getenv("CONTENT_LENGTH") ? atoi(getenv("CONTENT_LENGTH")) : 0;

  if (len && content_type)
    {
      /* XXX multipart/form-data not handled .. yet XXX */
      /* only handle application/x-www-form-urlencoded  */
      if (strncmp(content_type,"application/x-www-form-urlencoded",19) != 0)
	malformed_request();

      post_data = malloc( len+1 );
      len = fread(post_data, 1, len, stdin);
      post_data[len] = '\0';
      http_request_parse_params (req, post_data, '&');
  }

  if (getenv("HTTP_COOKIE"))
    http_request_parse_params (req, strdup((getenv("HTTP_COOKIE"))), '&'); 

  return req;
}

HttpResponse*
http_response_new(HttpRequest *req) 
{
  HttpResponse *res = NULL;

  res = malloc(sizeof(HttpResponse));
  memset(res, 0, sizeof(HttpResponse)); 

  res->status       = 200;
  res->status_str   = "OK";
  res->content_type = NULL;

  return res;
}

void
http_response_set_content_type(HttpResponse *res, char *type)
{
  res->content_type = strdup(type);
}

/* like Location: hello.html\r\n */
void
http_response_append_header(HttpResponse *res, char *header)
{
  if (res->extra_headers)
    {
      res->extra_headers 
	= realloc( res->extra_headers, 
		   strlen(res->extra_headers) + strlen(header) + 1);
      strcat(res->extra_headers, header);
    }
  else
    {
      res->extra_headers = strdup(header);
    }
}


void
http_response_printf_alloc_buffer(HttpResponse *res, int bytes)
{
  res->data_len_alloced += bytes;
  res->data = realloc( res->data, res->data_len_alloced );
}


void
http_response_printf(HttpResponse *res, const char *format, ...)
{
  va_list ap;
  char    *tmp = NULL;

  va_start(ap,format);
  vasprintf(&tmp, format, ap);
  va_end(ap);

  if ((res->data_len + strlen(tmp) + 1) < res->data_len_alloced)
    {
      if (res->data_len)
	memcpy(res->data + res->data_len - 1, tmp, strlen(tmp)+1);
      else
	memcpy(res->data, tmp, strlen(tmp)+1);
    }
  else if (!res->data_len) 		/* no data printed yet */
    {
      res->data = malloc(strlen(tmp)+1);
      memcpy(res->data, tmp, strlen(tmp)+1);
      res->data_len_alloced = strlen(tmp)+1;
    }
  else
    {
      res->data = realloc( res->data, res->data_len + strlen(tmp) );
      memcpy(res->data + res->data_len - 1, tmp, strlen(tmp)+1);
      res->data_len_alloced = res->data_len + strlen(tmp);
    }

  res->data_len = strlen(res->data)+1;      
  free(tmp);
}


void
http_response_set_status(HttpResponse *res,
			 int           status_code,
			 char         *status_desc) 
{
  res->status       = status_code;
  res->status_str   = strdup(status_desc);;
}


void
http_response_set_data(HttpResponse *res, void *data, int data_len)
{
  res->data     = (char *)data;
  res->data_len = data_len; 
}


void
http_response_send_headers(HttpResponse *res)
{
  printf("HTTP/1.0 %d %s\r\n", res->status, res->status_str);

  if (res->extra_headers) printf("%s", res->extra_headers);

  /* XXX didi likes cookies etc */

  printf("Content-Type: %s; charset=UTF-8\r\n", 
	 res->content_type == NULL ? "text/html" : res->content_type);

  if (res->data_len)
    {
      printf("Content-Length: %d\r\n", res->data_len);
      printf("Connection: close\r\n"); /* if fullHttpReply */
    }

  printf("\r\n");
}

void
http_response_send(HttpResponse *res)
{
  http_response_send_headers(res);

  if( res->data )
    fwrite(res->data, 1, res->data_len, stdout);
}

/*
** Maximum number of child processes that we can have running
** at one time before we start slowing things down.
*/
#define MAX_PARALLEL 5

/*
** Implement an HTTP server daemon.
*/
HttpRequest*
http_server(int iPort)
{
  int                listener;      /* The server socket */
  int                connection;    /* A socket for each connection */
  fd_set             readfds;       /* Set of file descriptors for select() */
  int                lenaddr;       /* Length of the inaddr structure */
  int                child;         /* PID of the child process */
  int                nchildren = 0; /* Number of child processes */
  struct timeval     delay;         /* How long to wait inside select() */
  struct sockaddr_in inaddr;        /* The socket address */
  int                reuse = 1;
  int                n = 0;

  memset(&inaddr, 0, sizeof(inaddr));
  inaddr.sin_family = AF_INET;
  inaddr.sin_addr.s_addr = INADDR_ANY;
  inaddr.sin_port = htons(iPort);
  listener = socket(AF_INET, SOCK_STREAM, 0);

  fprintf(stderr,"DidiWiki firing up ...\n");

  if( listener < 0 )
    {
      fprintf(stderr,"Can't create a socket\n");
      exit(1);
    }

#ifdef SO_REUSEADDR
  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)); 
#endif

  while (n < 10)
    {
      fprintf(stderr,"Attempting to use port %d .. ", iPort+n);

      inaddr.sin_port = htons(iPort + n);

      if( bind(listener, (struct sockaddr*)&inaddr, sizeof(inaddr)) < 0 )
	{
	  fprintf(stderr,"Failed! \n");
	  n++;
	  continue;
	}

      fprintf(stderr,"Success! \n");
      break;
    }

  if (n == 10)
    {
      fprintf(stderr,"Can't bind to any ports, giving up.\n");
      exit(1);
    }

  fprintf(stderr,"DidiWiki Started. Please point your browser at http://localhost:%i\n", iPort+n);

  listen(listener,10);

  while( 1 )
    {
      if( nchildren>MAX_PARALLEL )
	{
	  /* Slow down if connections are arriving too fast */
	  sleep( nchildren-MAX_PARALLEL );
	}

      delay.tv_sec = 60;
      delay.tv_usec = 0;
      FD_ZERO(&readfds);
      FD_SET( listener, &readfds);

      if( select( listener+1, &readfds, 0, 0, &delay) )
	{
	  lenaddr = sizeof(inaddr);
	  connection = accept(listener, (struct sockaddr*)&inaddr, &lenaddr);
	  if( connection>=0 )
	    {
	      child = fork();
	      if( child!=0 )
		{
		  if( child>0 ) 
		    nchildren++;
		  close(connection);
		}
	      else
		{
		  /* *child*, connect stdin/out to socket */
		  /* then return req object for caller to handle */
		  close(0);
		  dup(connection);
		  close(1);
		  dup(connection);
		  close(2);
		  dup(connection);
		  close(connection);
		  return http_request_new();
		}
	    }
	}
      
      /* Bury dead children */
      while( waitpid(0, 0, WNOHANG)>0 ) nchildren--;

      
    }

  /* NOT REACHED */  
  exit(1);
}
