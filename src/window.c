/* $Id$ */
/* Copyright (c) 2012-2022 Pierre Pronchery <khorben@defora.org> */
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
#include <gdk/gdkkeysyms.h>
#include <Desktop.h>
#include "auditor.h"
#include "window.h"
#include "../config.h"
#define _(string) gettext(string)
#define N_(string) (string)

#ifndef PROGNAME_AUDITOR
# define PROGNAME_AUDITOR	"auditor"
#endif


/* AuditorWindow */
/* private */
/* types */
struct _AuditorWindow
{
	Auditor * auditor;

	/* widgets */
	GtkWidget * window;
	GtkWidget * statusbar;
};


/* prototypes */
/* callbacks */
static void _auditorwindow_on_close(gpointer data);
static gboolean _auditorwindow_on_closex(gpointer data);
static void _auditorwindow_on_edit(gpointer data);
static void _auditorwindow_on_new(gpointer data);
static void _auditorwindow_on_preferences(gpointer data);

#ifndef EMBEDDED
/* menus */
/* file menu */
static void _auditorwindow_on_file_new(gpointer data);
static void _auditorwindow_on_file_edit(gpointer data);
static void _auditorwindow_on_file_close(gpointer data);

/* edit menu */
static void _auditorwindow_on_edit_select_all(gpointer data);
static void _auditorwindow_on_edit_delete(gpointer data);
static void _auditorwindow_on_edit_preferences(gpointer data);

/* view menu */
static void _auditorwindow_on_view_all_tasks(gpointer data);
static void _auditorwindow_on_view_completed_tasks(gpointer data);
static void _auditorwindow_on_view_remaining_tasks(gpointer data);

/* help menu */
static void _auditorwindow_on_help_about(gpointer data);
static void _auditorwindow_on_help_contents(gpointer data);
#endif

/* constants */
/* accelerators */
static const DesktopAccel _auditor_accel[] =
{
#ifdef EMBEDDED
	{ G_CALLBACK(_auditorwindow_on_close), GDK_CONTROL_MASK, GDK_KEY_W },
	{ G_CALLBACK(_auditorwindow_on_edit), GDK_CONTROL_MASK, GDK_KEY_E },
	{ G_CALLBACK(_auditorwindow_on_new), GDK_CONTROL_MASK, GDK_KEY_N },
	{ G_CALLBACK(_auditorwindow_on_preferences), GDK_CONTROL_MASK, GDK_KEY_P },
#endif
	{ NULL, 0, 0 }
};

#ifndef EMBEDDED
/* menubar */
static const DesktopMenu _file_menu[] =
{
	{ N_("_New"), G_CALLBACK(_auditorwindow_on_file_new), GTK_STOCK_NEW,
		GDK_CONTROL_MASK, GDK_KEY_N },
	{ N_("_Edit"), G_CALLBACK(_auditorwindow_on_file_edit), GTK_STOCK_EDIT,
		GDK_CONTROL_MASK, GDK_KEY_E },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Close"), G_CALLBACK(_auditorwindow_on_file_close), GTK_STOCK_CLOSE,
		GDK_CONTROL_MASK, GDK_KEY_W },
	{ NULL, NULL, NULL, 0, 0 }
};
static const DesktopMenu _edit_menu[] =
{
	{ N_("Select _All"), G_CALLBACK(_auditorwindow_on_edit_select_all),
#if GTK_CHECK_VERSION(2, 10, 0)
		GTK_STOCK_SELECT_ALL,
#else
		"edit-select-all",
#endif
		GDK_CONTROL_MASK, GDK_KEY_A },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Delete"), G_CALLBACK(_auditorwindow_on_edit_delete),
		GTK_STOCK_DELETE, 0, 0 },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Preferences"), G_CALLBACK(_auditorwindow_on_edit_preferences),
		GTK_STOCK_PREFERENCES, GDK_CONTROL_MASK, GDK_KEY_P },
	{ NULL, NULL, NULL, 0, 0 }
};
static const DesktopMenu _view_menu[] =
{
	{ N_("_All tasks"), G_CALLBACK(_auditorwindow_on_view_all_tasks), NULL, 0,
		0 },
	{ N_("_Completed tasks"), G_CALLBACK(
			_auditorwindow_on_view_completed_tasks), NULL, 0, 0 },
	{ N_("_Remaining tasks"), G_CALLBACK(
			_auditorwindow_on_view_remaining_tasks), NULL, 0, 0 },
	{ NULL, NULL, NULL, 0, 0 }
};
static const DesktopMenu _help_menu[] =
{
	{ N_("_Contents"), G_CALLBACK(_auditorwindow_on_help_contents),
		"help-contents", 0, GDK_KEY_F1 },
	{ N_("_About"), G_CALLBACK(_auditorwindow_on_help_about),
#if GTK_CHECK_VERSION(2, 6, 0)
		GTK_STOCK_ABOUT, 0, 0 },
#else
		NULL, 0, 0 },
#endif
	{ NULL, NULL, NULL, 0, 0 }
};
static const DesktopMenubar _menubar[] =
{
	{ N_("_File"), _file_menu },
	{ N_("_Edit"), _edit_menu },
	{ N_("_View"), _view_menu },
	{ N_("_Help"), _help_menu },
	{ NULL, NULL },
};
#endif


/* public */
/* functions */
/* auditorwindow_new */
AuditorWindow * auditorwindow_new(void)
{
	AuditorWindow * auditor;
	GtkAccelGroup * group;
	GtkWidget * vbox;
	GtkWidget * widget;

	if((auditor = malloc(sizeof(*auditor))) == NULL)
		return NULL;
	auditor->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	group = gtk_accel_group_new();
	auditor->auditor = auditor_new(auditor->window, group);
	/* check for errors */
	if(auditor->auditor == NULL)
	{
		auditorwindow_delete(auditor);
		g_object_unref(group);
		return NULL;
	}
	desktop_accel_create(_auditor_accel, auditor, group);
	gtk_window_add_accel_group(GTK_WINDOW(auditor->window), group);
	g_object_unref(group);
	gtk_window_set_default_size(GTK_WINDOW(auditor->window), 640, 480);
#if GTK_CHECK_VERSION(2, 6, 0)
	gtk_window_set_icon_name(GTK_WINDOW(auditor->window), "auditor");
#endif
	gtk_window_set_title(GTK_WINDOW(auditor->window), _("Auditor"));
	g_signal_connect_swapped(auditor->window, "delete-event", G_CALLBACK(
				_auditorwindow_on_closex), auditor);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#ifndef EMBEDDED
	/* menubar */
	widget = desktop_menubar_create(_menubar, auditor, group);
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, TRUE, 0);
#endif
	widget = auditor_get_widget(auditor->auditor);
	gtk_box_pack_start(GTK_BOX(vbox), widget, TRUE, TRUE, 0);
	/* statusbar */
	auditor->statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), auditor->statusbar, FALSE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(auditor->window), vbox);
	gtk_widget_show_all(auditor->window);
	return auditor;
}


/* auditorwindow_delete */
void auditorwindow_delete(AuditorWindow * auditor)
{
	if(auditor->auditor != NULL)
		auditor_delete(auditor->auditor);
	gtk_widget_destroy(auditor->window);
	free(auditor);
}


/* private */
/* functions */
/* callbacks */
/* auditorwindow_on_close */
static void _auditorwindow_on_close(gpointer data)
{
	AuditorWindow * auditor = data;

	_auditorwindow_on_closex(auditor);
}


/* auditorwindow_on_closex */
static gboolean _auditorwindow_on_closex(gpointer data)
{
	AuditorWindow * auditor = data;

	gtk_widget_hide(auditor->window);
	gtk_main_quit();
	return TRUE;
}


/* auditorwindow_on_edit */
static void _auditorwindow_on_edit(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_task_edit(auditor->auditor);
}


/* auditorwindow_on_new */
static void _auditorwindow_on_new(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_task_add(auditor->auditor, NULL);
}


/* auditorwindow_on_preferences */
static void _auditorwindow_on_preferences(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_show_preferences(auditor->auditor, TRUE);
}


#ifndef EMBEDDED
/* file menu */
/* auditorwindow_on_file_close */
static void _auditorwindow_on_file_close(gpointer data)
{
	AuditorWindow * auditor = data;

	_auditorwindow_on_close(auditor);
}


/* auditorwindow_on_file_edit */
static void _auditorwindow_on_file_edit(gpointer data)
{
	AuditorWindow * auditor = data;

	_auditorwindow_on_edit(auditor);
}


/* auditorwindow_on_file_new */
static void _auditorwindow_on_file_new(gpointer data)
{
	AuditorWindow * auditor = data;

	_auditorwindow_on_new(auditor);
}


/* edit menu */
/* auditorwindow_on_edit_delete */
static void _auditorwindow_on_edit_delete(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_task_delete_selected(auditor->auditor);
}


/* auditorwindow_on_edit_preferences */
static void _auditorwindow_on_edit_preferences(gpointer data)
{
	AuditorWindow * auditor = data;

	_auditorwindow_on_preferences(auditor);
}


/* auditorwindow_on_edit_select_all */
static void _auditorwindow_on_edit_select_all(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_task_select_all(auditor->auditor);
}


/* view menu */
/* auditorwindow_on_view_all_tasks */
static void _auditorwindow_on_view_all_tasks(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_set_view(auditor->auditor, AUDITOR_VIEW_ALL_TASKS);
}


/* auditorwindow_on_view_completed_tasks */
static void _auditorwindow_on_view_completed_tasks(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_set_view(auditor->auditor, AUDITOR_VIEW_COMPLETED_TASKS);
}


/* auditorwindow_on_view_remaining_tasks */
static void _auditorwindow_on_view_remaining_tasks(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_set_view(auditor->auditor, AUDITOR_VIEW_REMAINING_TASKS);
}


/* help menu */
/* auditorwindow_on_help_about */
static void _auditorwindow_on_help_about(gpointer data)
{
	AuditorWindow * auditor = data;

	auditor_about(auditor->auditor);
}


/* auditorwindow_on_help_contents */
static void _auditorwindow_on_help_contents(gpointer data)
{
	desktop_help_contents(PACKAGE, PROGNAME_AUDITOR);
}
#endif
