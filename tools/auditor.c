/* $Id$ */
/* Copyright (c) 2012-2020 Pierre Pronchery <khorben@defora.org> */
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



#ifndef EMBEDDED
# define EMBEDDED
#endif
#include <stdlib.h>
#include <Desktop/Mailer/plugin.h>

#include "../src/priority.c"
#include "../src/task.c"
#include "../src/taskedit.c"
#include "../src/auditor.c"


/* Auditor */
/* private */
/* types */
typedef struct _MailerPlugin AuditorPlugin;

struct _MailerPlugin
{
	MailerPluginHelper * helper;

	Auditor * auditor;

	/* widgets */
	GtkWidget * widget;
	GtkWidget * view;
};


/* protected */
/* prototypes */
/* plug-in */
static MailerPlugin * _auditor_init(MailerPluginHelper * helper);
static void _auditor_destroy(AuditorPlugin * auditor);
static GtkWidget * _auditor_get_widget(AuditorPlugin * auditor);


/* public */
/* variables */
/* plug-in */
MailerPluginDefinition plugin =
{
	"Auditor",
	"auditor",
	NULL,
	_auditor_init,
	_auditor_destroy,
	_auditor_get_widget,
	NULL
};


/* protected */
/* functions */
/* plug-in */
/* auditor_init */
static MailerPlugin * _auditor_init(MailerPluginHelper * helper)
{
	AuditorPlugin * auditor;
	GtkWidget * widget;
	size_t i;

	if((auditor = malloc(sizeof(*auditor))) == NULL)
		return NULL;
	if((auditor->auditor = auditor_new(NULL, NULL)) == NULL)
	{
		_auditor_destroy(auditor);
		return NULL;
	}
	auditor->helper = helper;
	auditor->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	widget = auditor_get_widget(auditor->auditor);
	gtk_box_pack_start(GTK_BOX(auditor->widget), widget, TRUE, TRUE, 0);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(auditor->auditor->view),
			FALSE);
	for(i = 0; i < TD_COL_COUNT; i++)
		if(auditor->auditor->columns[i] != NULL && i != TD_COL_TITLE)
			gtk_tree_view_column_set_visible(auditor->auditor->columns[i],
					FALSE);
	gtk_widget_show_all(auditor->widget);
	return auditor;
}


/* auditor_destroy */
static void _auditor_destroy(AuditorPlugin * auditor)
{
	if(auditor->auditor != NULL)
		auditor_delete(auditor->auditor);
	free(auditor);
}


/* auditor_get_widget */
static GtkWidget * _auditor_get_widget(AuditorPlugin * auditor)
{
	return auditor->widget;
}
