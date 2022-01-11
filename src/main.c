/* $Id$ */
/* Copyright (c) 2009-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Auditor */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <System.h>
#include "window.h"
#include "../config.h"
#define _(string) gettext(string)

/* constants */
#ifndef PROGNAME_AUDITOR
# define PROGNAME_AUDITOR	"auditor"
#endif
#ifndef PREFIX
# define PREFIX			"/usr/local"
#endif
#ifndef DATADIR
# define DATADIR		PREFIX "/share"
#endif
#ifndef LOCALEDIR
# define LOCALEDIR		DATADIR "/locale"
#endif


/* private */
/* prototypes */
static int _auditor(void);

static int _error(char const * message, int ret);
static int _usage(void);


/* functions */
/* auditor */
static int _auditor(void)
{
	AuditorWindow * auditor;

	if((auditor = auditorwindow_new()) == NULL)
		return error_print(PROGNAME_AUDITOR);
	gtk_main();
	auditorwindow_delete(auditor);
	return 0;
}


/* error */
static int _error(char const * message, int ret)
{
	fputs(PROGNAME_AUDITOR ": ", stderr);
	perror(message);
	return ret;
}


/* usage */
static int _usage(void)
{
	fprintf(stderr, _("Usage: %s\n"), PROGNAME_AUDITOR);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;

	if(setlocale(LC_ALL, "") == NULL)
		_error("setlocale", 1);
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	gtk_init(&argc, &argv);
	while((o = getopt(argc, argv, "")) != -1)
		switch(o)
		{
			default:
				return _usage();
		}
	if(optind != argc)
		return _usage();
	return (_auditor() == 0) ? 0 : 2;
}
