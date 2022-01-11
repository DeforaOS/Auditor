/* $Id$ */
/* Copyright (c) 2009-2015 Pierre Pronchery <khorben@defora.org> */
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



#ifndef AUDITOR_AUDITOR_H
# define AUDITOR_AUDITOR_H

# include "task.h"
# include <gtk/gtk.h>


/* Auditor */
/* types */
typedef struct _Auditor Auditor;

typedef enum _AuditorPriority
{
	AUDITOR_PRIORITY_UNKNOWN,
	AUDITOR_PRIORITY_LOW,
	AUDITOR_PRIORITY_MEDIUM,
	AUDITOR_PRIORITY_HIGH,
	AUDITOR_PRIORITY_URGENT
} AuditorPriority;

typedef enum _AuditorView
{
	AUDITOR_VIEW_ALL_TASKS = 0,
	AUDITOR_VIEW_COMPLETED_TASKS,
	AUDITOR_VIEW_REMAINING_TASKS
} AuditorView;
# define AUDITOR_VIEW_LAST AUDITOR_VIEW_REMAINING_TASKS
# define AUDITOR_VIEW_COUNT (AUDITOR_VIEW_LAST + 1)


/* functions */
Auditor * auditor_new(GtkWidget * window, GtkAccelGroup * group);
void auditor_delete(Auditor * auditor);

/* accessors */
AuditorView auditor_get_view(Auditor * auditor);
GtkWidget * auditor_get_widget(Auditor * auditor);
void auditor_set_view(Auditor * auditor, AuditorView view);

/* useful */
void auditor_about(Auditor * auditor);
int auditor_error(Auditor * auditor, char const * message, int ret);

void auditor_show_preferences(Auditor * auditor, gboolean show);

/* tasks */
Task * auditor_task_add(Auditor * auditor, Task * task);
void auditor_task_delete_selected(Auditor * auditor);
void auditor_task_remove_all(Auditor * auditor);

/* accessors */
void auditor_task_set_priority(Auditor * auditor, GtkTreePath * path,
		char const * priority);
void auditor_task_set_title(Auditor * auditor, GtkTreePath * path,
		char const * title);

void auditor_task_cursor_changed(Auditor * auditor);
void auditor_task_edit(Auditor * auditor);
int auditor_task_reload_all(Auditor * auditor);
void auditor_task_save_all(Auditor * auditor);
void auditor_task_select_all(Auditor * auditor);
void auditor_task_toggle_done(Auditor * auditor, GtkTreePath * path);

#endif /* !AUDITOR_AUDITOR_H */
