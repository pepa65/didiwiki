// wikitext.h
// DidiWiki - a small lightweight wiki engine
// Copyright 2004 Matthew Allum <mallum@o-hand.com> GPL-2+

#define HOMEDIR "." PACKAGE_TARNAME
#define CSS PACKAGE_TARNAME ".css"
#define FAVICON PACKAGE_TARNAME ".ico"
#define HOMEVAR "DIDIWIKIHOME"

#define HTMLHEAD "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n\
<html xmlns='http://www.w3.org/1999/xhtml'>\n\
<head>\n\
<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />\n\
<link rel='SHORTCUT ICON' href='/" FAVICON "' />\n\
<link media='all' href='/" CSS "' rel='stylesheet' type='text/css' />\n\
<title>%s</title>\n\
</head>\n\
<body>\n"

#define PAGEHEADER "\
<div id='header'>\n\
<form method='get' action='/Search'>\n\
<table border='0' width='100%%'>\n\
<tr>\n\
<td align='left' class='title'>%s</td>\n\
<td align='right' >\n\
<a class='button' href='?edit' title='Edit this page [Alt-J]' accesskey='j'>Edit</a> \n\
<a class='button' href='/Create' title='Create new page [Alt-C]' accesskey='c'>Create</a> \n\
<a class='button' href='/List' title='List files most recent changed pages [Alt-R]' accesskey='r' >List</a> \n\
<a class='button' href='/Home' title='Go to Home page [Alt-Z]' accesskey='z'>Home</a> \n\
<a class='button' href='/Help' title='Get help on wiki usage and formatting.'>Help</a> \n\
<input type='text' name='expr' size='15' placeholder='Search' title='Enter text to search for and press return.' /></td>\n\
</tr>\n\
</table>\n\
</form>\n\
</div>\n\
<div id='wikidata'>\n"

#define PAGEFOOTER "</div><div id='footer'><a title='" PACKAGE_NAME " home page' href='" PACKAGE_URL"' target='_blank'>" PACKAGE_NAME " version " VERSION "</a></div>\n"

#define CREATEFORM "\
<form method=POST action='/Create' name='create'>\n\
<input type='text' name='title' name='title' />\n\
<p><input type='submit' class='button' value='Create' /></p>\n\
</form>\n\
<script language='JavaScript'>\n\
<!--\n\
document.create.title.focus()\n\
//-->\n\
</script>\n"

#define EDITFORM "\
<form method=POST action='%s' name='editform'>\n\
<p><input type='submit' value='Save' class='button' title='Save [Alt-K]' accesskey='k'></p>\n\
<textarea name='wikitext' rows='40' cols='80' wrap='virtual'>%s</textarea>\n\
<p><input type='submit' value='Save' class='button' title='Save [Alt-K]'></p>\n\
</form>\n\
<script language='JavaScript'>\n\
<!--\n\
document.editform.wikitext.focus()\n\
document.editform.wikitext.scrollTop = document.editform.wikitext.scrollHeight;\n\
//-->\n\
</script>\n"

#define HOMETEXT "\
==Welcome to " PACKAGE_NAME "\n\
*^Feel free to erase the current content of this page to suit your needs!^*\n\
\n\
*" PACKAGE_NAME "* is a small and simple [http://en.wikipedia.org/wiki/Wiki Wiki] \
implementation. It's intended for personal note taking, to-do lists and \
any other uses you can think of. For an example what a " PACKAGE_NAME " entry \
looks like in text form you can [?edit edit] this page. Also see [Help] for \
infomation on usage and formatting rules.\n\
\n\
" PACKAGE_NAME " was written by [mailto:mallum@o-hand.com Matthew Allum] in C \
and is free software, released under the [http://www.gnu.org GNU] \
[http://www.gnu.org/copyleft/gpl.html GPL].\n\
This release is enhanced and maintained by [mailto:" PACKAGE_BUGREPORT " pepa65].\n\
" PACKAGE_NAME " uses a formatting style similar to that of \
[http://search.cpan.org/~ingy/Kwiki-0.39/lib/Kwiki.pm kwiki] and \
some webserver code from [http://www.cvstrac.org cvstrac]\n\
---\n\
==Technical notes\n\
Pages are stored in the directory ^" HOMEDIR "^ in your home, or in the \
directory specified in the variable " HOMEVAR " when " PACKAGE_NAME " is \
started up, like: ^" HOMEVAR "=/data/wiki " PACKAGE_TARNAME "^\n\
\n\
Besides the pages of the wiki, the CSS stylesheet ^" CSS "^ is stored there \
and can be modified.\n\
\n\
Also when a favicon with the name ^" FAVICON "^ is stored there, it will be \
used instead of the default built-in one.\n\
\n\
An upgraded version of " PACKAGE_NAME " might come with updated versions of \
the pages ^Help^ and ^Home^, or with an updated stylesheet ^" CSS "^ built \
into the binary "PACKAGE_TARNAME ". These updated versions will only be used \
if their old versions are removed from " HOMEDIR " -- be careful about this \
when you have modified these files to suit your requirements yourself!\n\
\n"

#define HELPTEXT "\
=Quick Guide\n\
Top toolbar usage:\n\
* [?edit Edit] Edit pages (formatting rules below)\n\
* [Create] Creates a new page\n\
* [List] Lists the most recent changed pages\n\
* [Home] Displays the home page\n\
* [Help] This help page\n\
\n\
Use the text entry box to perform a very simple keyword search on the \
Wiki contents. Hit return to start the search.\n\
\n\
---\n\
= _Formatting rules_\n\
Every new line means a paragraph break\n\
\n\
=Top level heading\n\
 =Top level heading (h1)\n\
\n\
==Second level heading\n\
 ==Second level heading (h2)\n\
\n\
===Third level heading\n\
 ===Third level heading (h3)\n\
\n\
====More levels\n\
 ====More levels: ==== (h4), ===== (h5), etc.\n\
---\n\
The horizontal lines in this page are made with 3 or more dashes:\n\
 --- Horizonal line (rest of the line is ignored)\n\
---\n\
Paragraphs are wrapped long lines.\n\
\n\
Newlines are inserted at the end of each line.\n\
\n\
 Paragraphs are wrapped long lines.\n\
 \n\
 Newlines are inserted at the end of each line.\n\
---\n\
*Bold text*, ^italic text^, _underscore text_ and -strikethrough-.\n\
 *Bold text*, ^italic text^, _underscore text_ and -strikethrough-.\n\
\n\
_^*Combination of bold and italics and underscore*^_\n\
 _^*Combination of bold and italics and underscore*^_\n\
---\n\
[Wiki links] are formed by a string surrounded by square brackets.\n\
 [Wiki links] are formed by a string surrounded by square brackets.\n\
\n\
External links begin with !http:// like http://www.freepan.org \
or with !https:// !ftp:// !file:// !mailto:// or !mailto:\n\
 External links begin with http:// like http://www.freepan.org\
 or with https:// ftp:// file:// mailto:// or mailto:\n\
\n\
External links can have a title after the first space, like [http://www.freepan.org FreePAN]\n\
 External links can have a title after the first space, like [http://www.freepan.org FreePAN]\n\
\n\
A space-less string prefaced with ! will not be linkified, bolded, italicized etc.\n\
like: !^no-i^ !*no-b* !_no !underline!_ ![NoWikiLink] !http://escap.ed etc.\n\
 A space-less string prefaced with ! will not be linkified, bolded, italicized etc.\n\
 like: !^no-i^ !*no-b* !_no !underline!_ ![NoWikiLink] !http://escap.ed etc.\n\
---\n\
Links to images display the image:\n\
\n\
http://www.gnu.org/graphics/heckert_gnu.small.png\n\
 http://www.gnu.org/graphics/heckert_gnu.small.png\n\
\n\
Links to images with title link to the image:\n\
[http://www.gnu.org/graphics/heckert_gnu.small.png Gnu]\n\
 [http://www.gnu.org/graphics/heckert_gnu.small.png Gnu]\n\
---\n\
Unordered lists begin with a '*' and the number of asterisks determines the level:\n\
* foo\n\
* bar\n\
** boom\n\
** bam\n\
* baz\n\
 * foo\n\
 * bar\n\
 ** boom\n\
 ** bam\n\
 * baz\n\
Ordered lists work the same, but use a '#'\n\
# foo\n\
## boom\n\
# bar\n\
---\n\
Tables begin the line with a '|' and before every new column after.\n\
| row:1,col:1 | row:1,col:2\n\
| row:2,col:1 | row:2,col:2\n\
 | row:1,col:1 | row:1,col:2\n\
 | row:2,col:1 | row:2,col:2\n\
---\n\
Inline html <i>is</i> <b>escaped</b>.\n\
 Inline html <i>is</i> <b>escaped</b>.\n\
---\n\
Lines prefixed with a space are unformatted (like the examples in light gray \
on this page)\n\
---\n"

#define STYLESHEET "\
body{font-family:Verdana, Arial, Helvetica, sans-serif; color:#000; margin:0;}\n\
#header{font-size:90%; background-color:#ddd; border:1px solid #aaa; padding:0em; margin:0.4em;}\n\
#header input{margin:0px; padding:0; background-color:#ffe; border:1px solid #aaa; font-size:90%;}\n\
#wikidata{font-size:100%; color:#111; margin:0.4em; padding:0.1em;}\n\
#footer{font-size:60%; background-color:#eee; padding-top: 2em; padding-bottom: 120em; margin-top:5em; width:100%; text-align:center;}\n\
h1,h2,h3,h4,h5,h6{margin-top:0; margin-bottom:0.5em;}\n\
pre{font-family:monospace; background-color:#eee; padding:0.5em;}\n\
.button{padding-left:0.3em; padding-right:0.3em; margin-left:0.3em; margin-right:0.3em;}\n\
.title{font-size:120%; font-weight:bold;}\n\
table.wiki{background-color:#fff; border:1px solid #aaa;}\n\
table.wiki td{background-color:#fff; border:1px solid #aaa;}\n\
hr{height:2px; color:#aaa; background-color:#aaa; border:0; margin:0.5em 0 0.5em 0;}\n\
form{border:none; margin:0;}\n\
textarea{border:1px solid #aaa; color:#000; background-color:#ffe; width:90%; padding:0.2em; overflow:auto;}\n\
input{margin-top:1px; padding:0 0.4em !important; background-color:#ffe; border:1px solid #aaa;}\n\
a,a:link{color:#333; text-decoration:none; border-bottom:1px #333 dotted; display:inline;}\n\
a:hover{color:#333; text-decoration:none; border-bottom:1px #000 solid;}\n\
a:visited{color:#333;}"

// transform a 16x16 image: ppmtowinicon -output favicon.ico favicon.ppm
// generate the codes using: xxd -i favicon.ico
static unsigned char FaviconData[] = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x02, 0x00, 0x01, 0x00,
	0x01, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
	0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xce, 0xce, 0x00, 0x00, 0x84, 0x84,
	0x00, 0x00, 0xa4, 0xa4, 0x00, 0x00, 0xa4, 0xa4, 0x00, 0x00, 0xa4, 0xa4,
	0x00, 0x00, 0xa4, 0xa4, 0x00, 0x00, 0x84, 0x84, 0x00, 0x00, 0xc7, 0xc7,
	0x00, 0x00, 0xe4, 0xe4, 0x00, 0x00, 0xe4, 0xe4, 0x00, 0x00, 0xcd, 0xcd,
	0x00, 0x00, 0xcf, 0xcf, 0x00, 0x00, 0x9f, 0x9f, 0x00, 0x00, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // to assemble this into a file: echo -ne "x\HHx\HH..." >favicon.ico

#define LINKSYMBOL "&#x1f517;"

