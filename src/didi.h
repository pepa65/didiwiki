// didi.h
// DidiWiki - a small lightweight wiki engine
// Copyright 2004 Matthew Allum <mallum@o-hand.com> GPL-2+

#include "config.h"

#if defined(__MINGW32__)

// This is broken
#undef DATADIR

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <dirent.h>

#else

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>

#endif

#include "util.h"
#include "http.h"
#include "wiki.h"

