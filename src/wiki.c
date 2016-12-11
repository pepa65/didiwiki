/*
 *  DidiWiki - a small lightweight wiki engine.
 *
 *  Copyright 2004 Matthew Allum <mallum@o-hand.com>
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
#include "wikitext.h"

static char *CssData=STYLESHEET;
static unsigned char *Favicon=FaviconData;
int FaviconSize=FAVICONLEN;

static char *get_line_from_string(char **lines)
{
	int i;
	char *z=*lines;
	if (z[0] == '\0') return NULL;
	for (i=0; z[i]; i++) if (z[i] == '\n')
	{
		if (i > 0 && z[i-1]=='\r') z[i-1]='\0';
		else z[i]='\0';
		i++;
		break;
	}
	// advance lines on
	*lines=&z[i];
	return z;
}

static char *check_for_link(char *line, int *skip_chars)
{
	char *start=line, *p=line, *url=NULL, *title=NULL, *result=NULL;
	int external=0;
	if (*p == '[')
	{
		*p='\0';
		p++;
		url=p;
		if (!strncasecmp(p, "http://", 7)
				|| !strncasecmp(p, "https://", 8)
				|| !strncasecmp(p, "ftp://", 6)
				|| !strncasecmp(p, "mailto://", 9)
				|| !strncasecmp(p, "mailto:", 7)
				|| !strncasecmp(p, "file://", 7)
				|| *p == '?' || *p == '/') // Some special local links starts with ? or /
		{ // External link, can be followed by title after space
			if (*p != '?' && *p != '/') external=1;
			while (*p != ']' && *p != '\0' && !isspace(*p)) p++;
			// title will come after space (if there is a space/title)
			if (isspace(*p))
			{ //title
				*p='\0';
				title=++p; 
				while (  *p != ']' && *p != '\0' ) p++;
				*p='\0';
				p++;
			}
			else // no title
			{
				*p='\0';
				p++;
			}
		}
		else // no title, simple wiki link
		{
			while (*p != ']' && *p != '\0') p++;
			*p='\0';
			p++;
		}
	}
	// No [ so check for bare external link
	else if (!strncasecmp(p, "http://", 7)
			|| !strncasecmp(p, "https://", 8)
			|| !strncasecmp(p, "ftp://", 6)
			|| !strncasecmp(p, "mailto://", 9)
			|| !strncasecmp(p, "mailto:", 7)
			|| !strncasecmp(p, "file://", 7))
	{ // Bare external link
		external=1;
		while (*p != '\0' && !isspace(*p)) p++;
		url=malloc(sizeof(char) * ((p-start)+2) );
		memset(url, 0, sizeof(char) * ((p-start)+2));
		strncpy(url, start, p-start);
		*start='\0';
	}
	if (url != NULL)
	{
		int len=strlen(url);
		*skip_chars=p-start;
		// is it an image?
		if (!title && (!strncmp(url+len-4, ".gif", 4)
				|| !strncmp(url+len-4, ".GIF", 4)
				|| !strncmp(url+len-4, ".PNG", 4)
				|| !strncmp(url+len-4, ".JPG", 4)
				|| !strncmp(url+len-5, ".JPEG", 5)
				|| !strncmp(url+len-4, ".png", 4)
				|| !strncmp(url+len-4, ".jpg", 4)
				|| !strncmp(url+len-5, ".jpeg", 5)))
			// display image
			asprintf(&result, "<img src='%s' border='0'>", url);
		else
		{ // no image
			if (!title) title=url;
			if (external)
				asprintf(&result,"<a title='%s' target='_blank' href='%s'>%s%s</a>", url, url, title, LINKSYMBOL);
			else asprintf(&result,"<a href='%s'>%s</a>", url, title);
		}
		return result;
	}
	return NULL;
}

static char *file_read(char *filename)
{
	struct stat st;
	FILE* fp;
	char* str;
	int len;
	// Get the file size
	if (stat(filename, &st)) return NULL;
	if (!(fp=fopen(filename, "rb"))) return NULL;
	str=(char *)malloc(sizeof(char)*(st.st_size+1));
	len=fread(str, 1, st.st_size, fp);
	if (len >= 0) str[len]='\0';
	fclose(fp);
	return str;
}

static int file_write(char *filename, char *data)
{
	FILE* fp;
	int bytes_written=0;
	int len=strlen(data);
	if (!(fp=fopen(filename, "wb"))) return -1;
	while (len > 0)
	{
		bytes_written=fwrite(data, sizeof(char), len, fp);
		len=len-bytes_written;
		data=data+bytes_written;
	}
	fclose(fp);
	return 1;
}

static int is_wiki_format_char_or_space(char c)
{
	if (isspace(c)) return 1;
	if (strchr("/*_-", c)) return 1;
	return 0;
}

void wiki_print_data_as_html(HttpResponse *res, char *raw_page_data)
{
	// accumalates non marked up text
	char *p=raw_page_data;
	// temporary scratch stuff
	// q is a pointer to the start of the next line
	char *q=p, *link, *line;
	int i, j, skip_chars;
	// flags, mainly for open tag states
	int bold_on=0;
	int italic_on=0;
	int underline_on=0;
	int strikethrough_on=0;
	int pre_on=0;
	int table_on=0;
#define ULIST 0
#define OLIST 1
#define NUM_LIST_TYPES 2
	struct { char ident; int depth; char *tag; } listtypes[]={
		{ '*', 0, "ul" },
		{ '#', 0, "ol" }
	};
	while ((line=get_line_from_string(&q)))
	{
		char *line_start=line;
		if (isspace(*line))
		{
			if (!pre_on) http_response_printf(res, "<pre>\n");
			pre_on=1;
			http_response_printf(res, "%s\n", line+1);
			continue;
		}
		if (!isspace(*line) && pre_on) {http_response_printf(res, "</pre>\n"); pre_on=0;}
		// Handle ordered & unordered list, code is a bit mental...
		for (i=0; i<NUM_LIST_TYPES; i++)
		{
			// extra checks avoid bolding
			if (*line == listtypes[i].ident
					&& (*(line+1) == listtypes[i].ident || isspace(*(line+1))))
			{ 	
				int item_depth=0;
				if (listtypes[!i].depth)
				{
					for (j=0; j<listtypes[!i].depth; j++)
						http_response_printf(res, "</%s>\n", listtypes[!i].tag);
					listtypes[!i].depth=0;
				}
				while (*line == listtypes[i].ident) {line++; item_depth++;}
				if (item_depth < listtypes[i].depth)
					for (j=0; j < (listtypes[i].depth-item_depth); j++)
						http_response_printf(res, "</%s>\n", listtypes[i].tag);
				else
					for (j=0; j < (item_depth-listtypes[i].depth); j++)
						http_response_printf(res, "<%s>\n", listtypes[i].tag);
				http_response_printf(res, "<li>");
				listtypes[i].depth=item_depth;
				goto line_content; // skip parsing any more initial chars
			}
			else if (listtypes[i].depth && !listtypes[!i].depth)
			{
				// close current list
				for (j=0; j<listtypes[i].depth; j++)
					http_response_printf(res, "</%s>\n", listtypes[i].tag);
				listtypes[i].depth=0;
			}
		}
		// Tables
		if (*line == '|')
		{
			if (table_on==0)
				http_response_printf(res, "<table class='wiki' cellspacing='0' cellpadding='4'>\n");
			table_on=1;
			http_response_printf(res, "<tr><td>");
			line++;
			goto line_content;
		}
		else
		{
			if (table_on)
			{
				http_response_printf(res, "</table>\n");
				table_on=0;
			}
		}
		int header_level=0, br=0;
		if (*line == '=')
		{
			while (*line == '=') {header_level++; line++;}
			if (header_level > 6) header_level=6;
			http_response_printf(res, "<h%d>", header_level);
			p=line;
		}
		else if (*line == '-' && *(line+1) == '-')
		{
			// rule
			http_response_printf(res, "<hr/>\n");
			while (*line == '-') line++;
		}
		else br=1;
line_content:
		// now process rest of the line
		p=line;
		while (*line != '\0')
		{
			if (*line == '!' && !isspace(*(line+1)))
			{ // escape next word - skip it
				*line='\0';
				http_response_printf(res, "%s", p);
				p=++line;
				while (*line != '\0' && !isspace(*line)) line++;
				if (*line == '\0') continue;
			}
			else if ((link=check_for_link(line, &skip_chars)) != NULL)
			{
				http_response_printf(res, "%s", p);
				http_response_printf(res, "%s", link);
				line+=skip_chars;
				p=line;
				continue;
			}
			else if (*line == '*')
			{ // bold
				if (line_start != line
						&& !is_wiki_format_char_or_space(*(line-1))
						&& !bold_on) {line++; continue;}
				if ((isspace(*(line+1)) && !bold_on)) {line++; continue;}
				*line='\0';
				http_response_printf(res, "%s%s", p, bold_on ? "</b>" : "<b>");
				bold_on^=1; // reset flag
				p=line+1;
			}
			else if (*line == '_')
			{ // underline
				if (line_start != line
						&& !is_wiki_format_char_or_space(*(line-1))
						&& !underline_on) {line++; continue;}
				if (isspace(*(line+1)) && !underline_on) {line++; continue;}
				*line='\0';
				http_response_printf(res, "%s%s", p, underline_on ? "</u>" : "<u>");
				underline_on^=1; // reset flag
				p=line+1;
			}
			else if (*line == '-' && *(line+1) != '-')
			{ // strikethrough
				if (line_start != line
						&& !is_wiki_format_char_or_space(*(line-1))
						&& !strikethrough_on) {line++; continue;}
				if (isspace(*(line+1)) && !strikethrough_on) {line++; continue;}
				*line='\0';
				http_response_printf(res, "%s%s", p, strikethrough_on ? "</del>" : "<del>");
				strikethrough_on^=1; // reset flag
				p=line+1;
			}
			else if (*line == '^')
			{ // italic
				if (line_start != line
						&& !is_wiki_format_char_or_space(*(line-1))
						&& !italic_on) {line++; continue;}
				if (isspace(*(line+1)) && !italic_on) {line++; continue;}
				*line='\0';
				http_response_printf(res, "%s%s", p, italic_on ? "</i>" : "<i>");
				italic_on^=1; // reset flag
				p=line+1;
			}
			else if (*line == '|' && table_on) // table column
			{
				*line='\0';
				http_response_printf(res, "%s", p);
				http_response_printf(res, "</td><td>\n");
				p=line+1;
			}
			line++;
		} // next word
		// accumalated text left over
		if (*p != '\0') http_response_printf(res, "%s", p);
		if (br) http_response_printf(res, "<br/>\n");
		// close any html tags that could be still open
		if (listtypes[ULIST].depth) http_response_printf(res, "</li>");
		if (listtypes[OLIST].depth) http_response_printf(res, "</li>");
		if (table_on) http_response_printf(res, "</td></tr>\n");
		if (header_level) http_response_printf(res, "</h%d>\n", header_level);
	} // next line
	// clean up anything thats still open
	if (pre_on) {http_response_printf(res, "</pre>\n"); pre_on=0;}
	// close any open lists
	for (i=0; i<listtypes[ULIST].depth; i++) http_response_printf(res, "\n</ul>\n");
	for (i=0; i<listtypes[OLIST].depth; i++) http_response_printf(res, "\n</ol>\n");
	// tables
	if (table_on) http_response_printf(res, "</table>\n");
}

int wiki_redirect(HttpResponse *res, char *location)
{
	char *location_enc = util_httpize(location);
	int header_len=strlen(location_enc)+14;
	char *header=alloca(sizeof(char)*header_len);
	snprintf(header, header_len, "Location: %s\r\n", location_enc);
	free(location_enc);
	http_response_append_header(res, header);
	http_response_printf(res, "<html>\n<p>Redirect to %s</p>\n</html>\n", location);
	http_response_set_status(res, 302, "Moved Temporarily");
	http_response_send(res);
	exit(0);
}

void wiki_show_page(HttpResponse *res, char *wikitext, char *page)
{
	char *html_clean_wikitext=NULL;
	http_response_printf_alloc_buffer(res, strlen(wikitext)*2);
	wiki_show_header(res, page);
	html_clean_wikitext=util_htmlize(wikitext, strlen(wikitext));
	wiki_print_data_as_html(res, html_clean_wikitext);
	wiki_show_footer(res);
	http_response_send(res);
	exit(0);
}

void wiki_show_edit_page(HttpResponse *res, char *wikitext, char *page)
{
	wiki_show_header(res, page);
	if (wikitext == NULL) wikitext="";
	http_response_printf(res, EDITFORM, page, wikitext);
	wiki_show_footer(res);
	http_response_send(res);
	exit(0);
}

void wiki_show_create_page(HttpResponse *res)
{
	wiki_show_header(res, "Create New Page");
	http_response_printf(res, "%s", CREATEFORM);
	wiki_show_footer(res);
	http_response_send(res);
	exit(0);
}

static int changes_compar(const struct dirent **d1, const struct dirent **d2)
{
	struct stat st1, st2;
	stat((*d1)->d_name, &st1);
	stat((*d2)->d_name, &st2);
	if (st1.st_mtime > st2.st_mtime) return 1;
	else return -1;
}

WikiPageList **wiki_get_pages(int *n_pages, char *expr)
{
	WikiPageList **pages;
	struct dirent **namelist;
	int n, i=0;
	struct stat st;
	n=scandir(".", &namelist, 0, (void *)changes_compar);
	pages=malloc(sizeof(WikiPageList*)*n);
	while (n--)
	{
		if ((namelist[n]->d_name)[0] == '.'
				|| !strcmp(namelist[n]->d_name, "favicon.ico")
				|| !strcmp(namelist[n]->d_name, "didiwiki.css")) goto cleanup;
		if (expr != NULL)
		{ // Super Simple Search
			char *data=NULL;
			if ((data=file_read(namelist[n]->d_name)) != NULL)
				if (strcasestr(data, expr) == NULL)
					if (strcmp(namelist[n]->d_name, expr) != 0) goto cleanup;
		}
		stat(namelist[n]->d_name, &st);
		// ignore anything but regular readable files
		if (S_ISREG(st.st_mode) && access(namelist[n]->d_name, R_OK) == 0)
		{
			pages[i]=malloc(sizeof(WikiPageList));
			pages[i]->name=strdup (namelist[n]->d_name);
			pages[i]->mtime=st.st_mtime;
			i++;
		}
cleanup:
		free(namelist[n]);
	}
	*n_pages=i;
	free(namelist);
	if (i==0) return NULL;
	return pages;
}

void wiki_show_changes_page(HttpResponse *res)
{
	WikiPageList **pages=NULL;
	int n_pages, i;
	wiki_show_header(res, "List");
	pages=wiki_get_pages(&n_pages, NULL);
	for (i=0; i<n_pages; i++)
	{
		struct tm *pTm;
		char datebuf[64];
		pTm=localtime(&pages[i]->mtime);
		strftime(datebuf, sizeof(datebuf), "%Y-%m-%d %H:%M", pTm);
		http_response_printf(res, "<a href='%s'>%s</a> %s<br />\n",
				pages[i]->name,
				pages[i]->name,
				datebuf);
	}
	wiki_show_footer(res);
	http_response_send(res);
	exit(0);
}

void wiki_show_search_results_page(HttpResponse *res, char *expr)
{
	WikiPageList **pages=NULL;
	int n_pages, i;
	if (expr == NULL || strlen(expr) == 0)
	{
		wiki_show_header(res, "Search");
		http_response_printf(res, "No Search Terms supplied");
		wiki_show_footer(res);
		http_response_send(res);
		exit(0);
	}
	pages=wiki_get_pages(&n_pages, expr);
	if (pages)
	{
		wiki_show_header(res, "Search");
		// if only one page is found, redirect to it
		if (n_pages == 1) wiki_redirect(res, pages[0]->name);
		for (i=0; i<n_pages; i++)
			http_response_printf(res, "<a href='%s'>%s</a><br />\n", pages[i]->name, pages[i]->name);
	}
	else
	{
		wiki_show_header(res, "Search");
		http_response_printf(res, "No matches");
	}
	wiki_show_footer(res);
	http_response_send(res);
	exit(0);
}

void wiki_show_template(HttpResponse *res, char *template_data)
{
	// 4 templates: header.html footer.html header-noedit.html footer-noedit.html
	// vars: $title (page title), $include() (?), $pages
}

void wiki_show_header(HttpResponse *res, char *page_title)
{
	http_response_printf(res, HTMLHEAD, page_title);
	http_response_printf(res, PAGEHEADER, page_title);
}

void wiki_show_footer(HttpResponse *res)
{
	http_response_printf(res, "%s", PAGEFOOTER);
	http_response_printf(res, "</body>\n</html>\n");
}

void wiki_handle_http_request(HttpRequest *req)
{
	HttpResponse *res=http_response_new(req);
	char *page=http_request_get_path_info(req);
	char *command=http_request_get_query_string(req);
	char *wikitext="";
	util_dehttpize(page); // remove any encoding on the requested page name
	if (!strcmp(page, "/"))
	{
		if (access("Home", R_OK) != 0) wiki_redirect(res, "/Home?create");
		page="/Home";
	}
	if (!strcmp(page, "/didiwiki.css"))
	{
		// Return CSS page
		http_response_set_content_type(res, "text/css");
		http_response_printf(res, "%s", CssData);
		http_response_send(res);
		exit(0);
	}
	if (!strcmp(page, "/favicon.ico"))
	{
		// Return favicon
		http_response_set_content_type(res, "image/ico");
		http_response_set_data(res, Favicon, FaviconSize);
		http_response_send(res);
		exit(0);
	}
	page++; // skip slash
	// Safety: issue a malformed request for any paths (there shouldn't need to be any)
	if (strchr(page, '/'))
	{
		http_response_set_status(res, 404, "Not Found");
		http_response_printf(res, "<html><body>404 Not Found</body></html>\n");
		http_response_send(res);
		exit(0);
	}
	if (!strcmp(page, "List"))
	{
		// TODO list recent changes
		wiki_show_changes_page(res);
	}
	else if (!strcmp(page, "Search"))
		wiki_show_search_results_page(res, http_request_param_get(req, "expr"));
	else if (!strcmp(page, "Create"))
	{
		if ((wikitext=http_request_param_get(req, "title")) != NULL)
		{
			// create page and redirect
			wiki_redirect(res, http_request_param_get(req, "title"));
		}
		else
		{
			// show create page form
			wiki_show_create_page(res);
		}
	}
	else
	{
		// TODO: dont blindly write wikitext data to disk
		if ((wikitext=http_request_param_get(req, "wikitext")) != NULL)
			file_write(page, wikitext);
		if (access(page, R_OK) == 0) 	// page exists
		{
			wikitext=file_read(page);
			if (!strcmp(command, "edit"))
			{
				// print edit page
				wiki_show_edit_page(res, wikitext, page);
			}
			else wiki_show_page(res, wikitext, page);
		}
		else if (!strcmp(command, "create")) wiki_show_edit_page(res, NULL, page);
		else
		{
			char buf[1024];
			snprintf(buf, 1024, "%s?create", page);
			wiki_redirect(res, buf);
		}
	}
}

int wiki_init(void)
{
	char datadir[512]={ 0 };
	struct stat st;
	if (getenv("DIDIWIKIHOME")) snprintf(datadir, 512, getenv("DIDIWIKIHOME"));
	else
	{
		if (getenv("HOME") == NULL)
		{
			fprintf(stderr, "Unable to get home directory, is HOME set?\n");
			exit(1);
		}
		snprintf(datadir, 512, "%s/.didiwiki", getenv("HOME"));
	}
	// Check if ~/.didiwiki exists and create if not
	if (stat(datadir, &st) != 0)
		if (mkdir(datadir, 0755) == -1)
		{
			fprintf(stderr, "Unable to create '%s', giving up.\n", datadir);
			exit(1);
		}
	chdir(datadir);
	// Write Default Help + Home page and stylesheet if it doesn't exist
	if (access("Help", R_OK) != 0) file_write("Help", HELPTEXT);
	if (access("Home", R_OK) != 0) file_write("Home", HOMETEXT);
	if (access("didiwiki.css", R_OK) != 0) file_write("didiwiki.css", CssData);
	// Read in optional stylesheet
	if (access("didiwiki.css", R_OK) == 0) CssData=file_read("didiwiki.css");
	// Read optional favicon
	if (access("favicon.ico", R_OK) == 0)
	{
		struct stat st;
		FILE* fp;
		if (!stat("favicon.ico", &st) && (fp=fopen("favicon.ico", "rb")))
		{
			Favicon=(char *)malloc(sizeof(char)*(st.st_size+1));
			FaviconSize=fread(Favicon, 1, st.st_size, fp);
			fclose(fp);
		}
	}
	return 1;
}
