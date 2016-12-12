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

#ifndef _HAVE_WIKI_TEXT_HEADER
#define _HAVE_WIKI_TEXT_HEADER
#define NAME "DidiWiki"
#define HOME "https://github.com/pepa65/didiwiki"

#define PAGEHEADER "\
<div id='header'>\n\
<form method='get' action='/Search'>\n\
<table border='0' width='100%%'>\n\
<tr>\n\
<td align='left' class='title'>%s</td>\n\
<td align='right' >\n\
<a class='button' href='?edit' title='Edit this page [alt-j]' accesskey='j'>Edit</a> \n\
<a class='button' href='/Home' title='Go to Home page [alt-z]' accesskey='z'>Home</a> \n\
<a class='button' href='/List' title='List files most recent changed pages [alt-r]' accesskey='r' >List</a> \n\
<a class='button' href='/Create' title='Create new page [alt-c]' accesskey='c'>Create</a> \n\
<a class='button' href='/Help' title='Get help on wiki usage and formatting.'>Help</a> \n\
<input type='text' name='expr' size='15' placeholder='Search' title='Enter text to search for and press return.' /></td>\n\
</tr>\n\
</table>\n\
</form>\n\
</div>\n\
<div id='wikidata'>\n"

#define PAGEFOOTER "</div><div id='footer'><a title='" NAME " home page' href='" HOME "' target='_blank'>" NAME " version " VERSION "</a></div>\n"

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
<textarea name='wikitext' rows='40' cols='80' wrap='virtual'>%s</textarea>\n\
<p><input type='submit' value='Save' class='button' title='[alt-k]' accesskey='k'></p>\n\
</form>\n\
<script language='JavaScript'>\n\
<!--\n\
document.editform.wikitext.focus()\n\
document.editform.wikitext.scrollTop = document.editform.wikitext.scrollHeight;\n\
//-->\n\
</script>\n"

#define HOMETEXT "\
==Welcome to " NAME "\n\
*" NAME "* is a small and simple [http://en.wikipedia.org/wiki/Wiki Wiki]\n\
implementaion. It's intended for personal note taking, to-do lists and any other uses you can think of.\n\
\n\
For an example what a " NAME " entry looks like in text form you can [?edit edit] \
this page. Also see [Help] for infomation on usage and formatting rules.\n\
\n\
" NAME " was written by [mailto:mallum@o-hand.com Matthew Allum] in C and is \
free software, released under the [http://www.gnu.org GNU] [http://www.gnu.org/copyleft/gpl.html GPL]. \
This release was enhanced by [mailto:solusos@passchier.net pepa65].\n\
" NAME " uses a formatting style similar to that of \
[http://search.cpan.org/~ingy/Kwiki-0.39/lib/Kwiki.pm kwiki] and \
some webserver code from [http://www.cvstrac.org cvstrac]\n"

#define HELPTEXT "\
=Quick Guide\n\
Top toolbar usage:\n\
* [?edit Edit] Edit pages (formatting rules below)\n\
* [Home] Displays the home page\n\
* [List] Lists the most recent changed pages\n\
* [Create] Creates a new page\n\
* [Help] This help page\n\
\n\
Use the text entry box to perform a very simple keyword search on the \
Wiki contents. Hit return to start the search.\n\
\n\
----\n\
= _Formatting rules_\n\
 = _Formatting rules_\n\
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
----\n\
The horizontal lines in this page are made with 4 or more dashes:\n\
 ---- Horizonal line\n\
----\n\
Paragraphs are wrapped long lines.\n\
\n\
Newlines are inserted at the end of each line.\n\
\n\
 Paragraphs are wrapped long lines.\n\
 \n\
 Newlines are inserted at the end of each line.\n\
----\n\
*Bold text*, ^italic text^, _underscore text_ and -strikethrough-.\n\
 *Bold text*, ^italic text^, _underscore text_ and -strikethrough-.\n\
\n\
^*Combination of bold and italics*^\n\
 ^*Combination of bold and italics*^\n\
----\n\
[WikiLinks] are formed by a string surrounded by square brackets.\n\
 [WikiLinks] are formed by a string surrounded by square brackets.\n\
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
like: !^no-i^ !*no-b* !_no !underline!_ ![NoWikiLink] !http//escap.ed etc.\n\
 A space-less string prefaced with ! will not be linkified, bolded, italicized etc.\n\
 like: !^no-i^ !*no-b* !_no !underline!_ ![NoWikiLink] !http//escap.ed etc.\n\
----\n\
Links to images display the image:\n\
\n\
http://www.gnu.org/graphics/heckert_gnu.small.png\n\
 http://www.gnu.org/graphics/heckert_gnu.small.png\n\
\n\
Links to images with title link to the image:\n\
[http://www.gnu.org/graphics/heckert_gnu.small.png Gnu]\n\
 [http://www.gnu.org/graphics/heckert_gnu.small.png Gnu]\n\
----\n\
Unordered lists begin with a * and the number of asterisks determines the level:\n\
* foo\n\
* bar\n\
** boom\n\
** bam\n\
* baz\n\
\n\
 * foo\n\
 * bar\n\
 ** boom\n\
 ** bam\n\
 * baz\n\
Ordered lists work the same, but use a '#'\n\
----\n\
Tables begin the line with a '|' and before every new column after.\n\
| row:1,col:1 | row:1,col:2\n\
| row:2,col:1 | row:2,col:2\n\
 | row:1,col:1 | row:1,col:2\n\
 | row:2,col:1 | row:2,col:2\n\
---\n\
Inline html <i>is</i> <b>escaped</b>.\n\
 Inline html <i>is</i> <b>escaped</b>.\n\
----\n\
Lines prefixed with a space are unformatted (like the examples in light gray on this page)\n\
----\n"

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
a,ulink{color:#333; text-decoration:none; border-bottom:1px #333 dotted; display:inline;}\n\
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
#define FAVICONLEN 198;

#define LINKSYMBOL "&#x1f517;"

#endif
