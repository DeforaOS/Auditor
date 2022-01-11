/* $Id$ */
/* Copyright (c) 2010-2016 Pierre Pronchery <khorben@defora.org> */
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



#include <stdlib.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <Desktop.h>
#include "priority.h"
#include "taskedit.h"
#define _(string) gettext(string)


/* TaskEdit */
/* private */
/* types */
struct _TaskEdit
{
	Auditor * auditor;
	Task * task;

	/* widgets */
	GtkWidget * window;
	GtkWidget * title;
	GtkWidget * priority;
	GtkWidget * description;
};


/* public */
/* functions */
/* task_new */
static void _on_taskedit_cancel(gpointer data);
static void _on_taskedit_ok(gpointer data);

TaskEdit * taskedit_new(Auditor * auditor, Task * task)
{
	TaskEdit * taskedit;
	char buf[80];
	size_t i;
	GtkSizeGroup * group;
	GtkWidget * vbox;
	GtkWidget * hbox;
	GtkWidget * widget;
	GtkWidget * entry;
	GtkWidget * bbox;
	GtkWidget * scrolled;
	char const * description;

	if((taskedit = malloc(sizeof(*taskedit))) == NULL)
		return NULL;
	taskedit->auditor = auditor;
	taskedit->task = task;
	taskedit->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	snprintf(buf, sizeof(buf), "%s%s", _("Edit task: "), task_get_title(
				task));
	gtk_window_set_default_size(GTK_WINDOW(taskedit->window), 300, 400);
	gtk_window_set_title(GTK_WINDOW(taskedit->window), buf);
	g_signal_connect_swapped(taskedit->window, "delete-event", G_CALLBACK(
				_on_taskedit_cancel), taskedit);
	group = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	/* title */
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	widget = gtk_label_new(_("Title:"));
#if GTK_CHECK_VERSION(3, 0, 0)
	g_object_set(widget, "halign", GTK_ALIGN_START, NULL);
#else
	gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
#endif
	gtk_size_group_add_widget(group, widget);
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
	taskedit->title = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(taskedit->title), task_get_title(task));
	gtk_box_pack_start(GTK_BOX(hbox), taskedit->title, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	/* priority */
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	widget = gtk_label_new(_("Priority:"));
#if GTK_CHECK_VERSION(3, 0, 0)
	g_object_set(widget, "halign", GTK_ALIGN_START, NULL);
#else
	gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
#endif
	gtk_size_group_add_widget(group, widget);
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
#if GTK_CHECK_VERSION(3, 0, 0)
	taskedit->priority = gtk_combo_box_text_new_with_entry();
	for(i = 0; priorities[i].title != NULL; i++)
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(
					taskedit->priority),
				_(priorities[i].title));
#else
	taskedit->priority = gtk_combo_box_entry_new_text();
	for(i = 0; priorities[i].title != NULL; i++)
		gtk_combo_box_append_text(GTK_COMBO_BOX(taskedit->priority),
				_(priorities[i].title));
#endif
	entry = gtk_bin_get_child(GTK_BIN(taskedit->priority));
	gtk_entry_set_text(GTK_ENTRY(entry), task_get_priority(task));
	gtk_box_pack_start(GTK_BOX(hbox), taskedit->priority, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	/* description */
	widget = gtk_label_new(_("Description:"));
#if GTK_CHECK_VERSION(3, 0, 0)
	g_object_set(widget, "halign", GTK_ALIGN_START, NULL);
#else
	gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
#endif
	gtk_size_group_add_widget(group, widget);
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, TRUE, 0);
	scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled),
			GTK_SHADOW_IN);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	taskedit->description = gtk_text_view_new();
	if((description = task_get_description(task)) != NULL)
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(
						taskedit->description)),
				description, -1);
	gtk_container_add(GTK_CONTAINER(scrolled), taskedit->description);
	gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);
	bbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_set_spacing(GTK_BOX(bbox), 4);
	widget = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect_swapped(widget, "clicked", G_CALLBACK(
				_on_taskedit_cancel), taskedit);
	gtk_container_add(GTK_CONTAINER(bbox), widget);
	widget = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect_swapped(widget, "clicked", G_CALLBACK(_on_taskedit_ok),
			taskedit);
	gtk_container_add(GTK_CONTAINER(bbox), widget);
	gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, TRUE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(taskedit->window), 4);
	gtk_container_add(GTK_CONTAINER(taskedit->window), vbox);
	gtk_widget_show_all(taskedit->window);
	return taskedit;
}

static void _on_taskedit_cancel(gpointer data)
{
	TaskEdit * taskedit = data;

	taskedit_delete(taskedit);
}

static void _on_taskedit_ok(gpointer data)
{
	TaskEdit * taskedit = data;
	GtkWidget * entry;
	GtkTextBuffer * tbuf;
	GtkTextIter start;
	GtkTextIter end;
	gchar * description;
	
	task_set_title(taskedit->task, gtk_entry_get_text(GTK_ENTRY(
					taskedit->title)));
	entry = gtk_bin_get_child(GTK_BIN(taskedit->priority));
	task_set_priority(taskedit->task, gtk_entry_get_text(GTK_ENTRY(entry)));
	tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(taskedit->description));
	gtk_text_buffer_get_start_iter(tbuf, &start);
	gtk_text_buffer_get_end_iter(tbuf, &end);
	description = gtk_text_buffer_get_text(tbuf, &start, &end, FALSE);
	task_set_description(taskedit->task, description);
	g_free(description);
	task_save(taskedit->task);
	auditor_task_reload_all(taskedit->auditor); /* XXX violent solution */
	_on_taskedit_cancel(taskedit);
}


/* taskedit_delete */
void taskedit_delete(TaskEdit * taskedit)
{
	gtk_widget_destroy(taskedit->window);
	free(taskedit);
}
