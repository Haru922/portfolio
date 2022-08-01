/*
 * Copyright (C) 2021 Gooroom <gooroom@gooroom.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <json-c/json.h>

#include "gooroom-feedback-utils.h"
#include "gooroom-feedback-list-row.h"
#include "gooroom-feedback-app-window.h"
#include "gooroom-feedback-write-dialog.h"

#define GFB_RESOURCE_NEW "/kr/gooroom/gooroom-feedback-client/ic-new.svg"

enum
{
  GFB_DATE = 0,
  GFB_TITLE,
  GFB_CATEGORY,
  GFB_DESC,
  GFB_RESPONSE,
  GFB_COLUMNS
};

struct _GooroomFeedbackAppWindowPrivate
{
  GtkWidget *gfb_listbox;
  GtkWidget *gfb_button_new_dialog;
  GtkWidget *gfb_history_stack;
  GtkWidget *gfb_history_none_box;
  GtkWidget *gfb_history_scrolled_window;

  gboolean use_ampm;
  GFileMonitor *monitor;

  GSettings  *clock_settings;
};

G_DEFINE_TYPE_WITH_PRIVATE (GooroomFeedbackAppWindow, gooroom_feedback_app_window, GTK_TYPE_APPLICATION_WINDOW);



/*
 * Translate @str according to the locale defined by LC_TIME; unlike
 * dcgettext(), the translation is still taken from the LC_MESSAGES
 * catalogue and not the LC_TIME one.
 */
static const gchar *
translate_time_format_string (const char *str)
{
  const char *locale = g_getenv ("LC_TIME");
  const char *res;
  char *sep;
  locale_t old_loc;
  locale_t loc = (locale_t)0;

  if (locale)
    loc = newlocale (LC_MESSAGES_MASK, locale, (locale_t)0);

  old_loc = uselocale (loc);

  sep = strchr (str, '\004');
  res = g_dpgettext (GETTEXT_PACKAGE, str, sep ? sep - str + 1 : 0);

  uselocale (old_loc);

  if ((locale_t)0 != loc)
    freelocale (loc);

  return res;
}

static void
clear_history (GooroomFeedbackAppWindow *win)
{
  GList *children, *l = NULL;

  children = gtk_container_get_children (GTK_CONTAINER (win->priv->gfb_listbox));
  for (l = children; l; l = l->next)
    gtk_widget_destroy (GTK_WIDGET (l->data));
  g_list_free (children);
}

static gboolean
gfb_update_history_cb (gpointer user_data)
{
  gint lines = 0;
  gsize length = 0;
  gboolean ret = FALSE;
  gchar *contents = NULL;
  gchar *log_path = NULL;
  json_object *jso = NULL;
  GtkWidget *visible_child = NULL;

  GooroomFeedbackAppWindow *win = GOOROOM_FEEDBACK_APP_WINDOW (user_data);
  GooroomFeedbackAppWindowPrivate *priv = win->priv;
  
  clear_history (win);

  log_path = get_feedback_log_path ();
  if (!g_file_test (log_path, G_FILE_TEST_EXISTS))
    goto error;

  ret = g_file_get_contents (log_path, &contents, &length, NULL);
  if (!ret || !length)
    goto error;

  jso = json_object_from_file (log_path);
  if (json_type_array != json_object_get_type (jso)) {
    g_warning ("Invalid log file format.");
    g_file_set_contents (log_path, "", -1, NULL);
    goto error;
  }

  int i = 0, len = 0;;
  len = json_object_array_length (jso);
  for (i = 0; i < len; i++) {
    GtkWidget *row = NULL;
    gint64 datetime = 0;
    gchar *time_str = NULL;
    const gchar *vals[GFB_COLUMNS] = {0,};
    json_object *p_obj[GFB_COLUMNS] = {0,};

    json_object *obj = json_object_array_get_idx (jso, i);

    p_obj[GFB_DATE]     = JSON_OBJECT_GET (obj, "datetime");
    p_obj[GFB_TITLE]    = JSON_OBJECT_GET (obj, "title");
    p_obj[GFB_CATEGORY] = JSON_OBJECT_GET (obj, "category");
    p_obj[GFB_DESC]     = JSON_OBJECT_GET (obj, "description");
    p_obj[GFB_RESPONSE] = JSON_OBJECT_GET (obj, "response");

    datetime           = p_obj[GFB_DATE] ? json_object_get_int64 (p_obj[GFB_DATE]) : -1;
    vals[GFB_TITLE]    = p_obj[GFB_TITLE] ? json_object_get_string (p_obj[GFB_TITLE]) : "";
    vals[GFB_CATEGORY] = p_obj[GFB_CATEGORY] ? json_object_get_string (p_obj[GFB_CATEGORY]) : "";
    vals[GFB_DESC]     = p_obj[GFB_DESC] ? json_object_get_string (p_obj[GFB_DESC]) : "";
    vals[GFB_RESPONSE] = p_obj[GFB_RESPONSE] ? json_object_get_string (p_obj[GFB_RESPONSE]) : "";

	if (g_str_equal (vals[GFB_RESPONSE], "SUCCESS")) {
		if (datetime >= 0) {
		GDateTime *dt = g_date_time_new_from_unix_local (datetime);
		if (priv->use_ampm) // 12-hour mode
			time_str = g_date_time_format (dt, translate_time_format_string (N_("%B %-d %Y   %l:%M:%S %p")));
		else
			time_str = g_date_time_format (dt, translate_time_format_string (N_("%B %-d %Y   %T")));
		g_date_time_unref (dt);
		}

		row = g_object_new (GOOROOM_TYPE_FEEDBACK_LIST_ROW,
							"datetime", time_str,
							"title", vals[GFB_TITLE],
							"category", vals[GFB_CATEGORY],
							"description", vals[GFB_DESC],
							"response", vals[GFB_RESPONSE],
							NULL);
		gtk_list_box_prepend (GTK_LIST_BOX (priv->gfb_listbox), row);
		lines++;

		g_clear_pointer (&time_str, g_free);
	}
  }
  gtk_widget_show_all (priv->gfb_listbox);


error:
  visible_child = (0 == lines) ? priv->gfb_history_none_box : priv->gfb_history_scrolled_window;
  gtk_stack_set_visible_child (GTK_STACK (priv->gfb_history_stack), visible_child);

  g_clear_pointer (&log_path, g_free);
  g_clear_pointer (&contents, g_free);

  json_object_put (jso);

  return FALSE;
}

static void
new_button_clicked_cb (GtkButton *widget,
                       gpointer   user_data)
{
  GtkWidget *dialog;
  GtkWidget *toplevel;

  toplevel = gtk_widget_get_toplevel (GTK_WIDGET (widget));

  dialog = gooroom_feedback_write_dialog_new (GTK_WINDOW (toplevel));
  gtk_widget_show (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void
row_activated_cb (GtkListBox    *listbox,
                  GtkListBoxRow *row,
                  gpointer       user_data)
{
  gooroom_feedback_list_row_expand (GOOROOM_FEEDBACK_LIST_ROW (row));
}

//static int
//row_sort_func (GooroomFeedbackListRow *a, GooroomFeedbackListRow *b, gpointer data)
//{
//  return b->priv->message->time - a->priv->message->time;
//}

static void
log_file_changed_cb (GFileMonitor      *monitor,
                     GFile             *file,
                     GFile             *other_file,
                     GFileMonitorEvent  event_type,
                     gpointer           user_data)
{
  g_timeout_add (100, G_SOURCE_FUNC (gfb_update_history_cb), user_data);
}

static void
clock_settings_changed_cb (GSettings *settings, gchar *key, gpointer user_data)
{
  gint clock_format;
  GooroomFeedbackAppWindow *win = GOOROOM_FEEDBACK_APP_WINDOW (user_data);
  GooroomFeedbackAppWindowPrivate *priv = win->priv;

  clock_format = g_settings_get_enum (priv->clock_settings, "clock-format");

  priv->use_ampm = (1 == clock_format);

  g_timeout_add (100, G_SOURCE_FUNC (gfb_update_history_cb), user_data);
}

static void
gooroom_feedback_app_window_finalize (GObject *object)
{
  GooroomFeedbackAppWindow *win = GOOROOM_FEEDBACK_APP_WINDOW (object);

  g_clear_object (&win->priv->monitor);
  g_clear_object (&win->priv->clock_settings);

  G_OBJECT_CLASS (gooroom_feedback_app_window_parent_class)->finalize (object);
}

static void
gooroom_feedback_app_window_init (GooroomFeedbackAppWindow *win)
{
  GFile *file;
  GError *error = NULL;
  gchar *log_path = NULL;
  GooroomFeedbackAppWindowPrivate *priv = NULL;

  priv = win->priv = gooroom_feedback_app_window_get_instance_private (win);

  gtk_widget_init_template (GTK_WIDGET (win));

//  gtk_list_box_set_sort_func (GTK_LIST_BOX (priv->gfb_listbox),
//                              (GtkListBoxSortFunc)row_sort_func, priv->gfb_listbox, win);
  gtk_list_box_set_activate_on_single_click (GTK_LIST_BOX (priv->gfb_listbox), TRUE);

  g_signal_connect (G_OBJECT (priv->gfb_listbox),
                    "row-activated",
                    G_CALLBACK (row_activated_cb), win);

  g_signal_connect (priv->gfb_button_new_dialog,
                    "clicked",
                    G_CALLBACK (new_button_clicked_cb),
                    win);

  g_timeout_add (100,
                 G_SOURCE_FUNC (gfb_update_history_cb),
                 win);

  priv->clock_settings = g_settings_new ("org.gnome.desktop.interface");

  g_signal_connect (priv->clock_settings, "changed::clock-format",
                    G_CALLBACK (clock_settings_changed_cb), win);


  log_path = get_feedback_log_path ();
  file = g_file_new_for_path (log_path);
  priv->monitor = g_file_monitor_file (file, G_FILE_MONITOR_NONE, NULL, &error);
  if (error) {
    g_error_free (error);
  } else {
    g_signal_connect (priv->monitor, "changed", G_CALLBACK (log_file_changed_cb), win);
  }
  g_object_unref (file);
  g_clear_pointer (&log_path, g_free);
}

static void
gooroom_feedback_app_window_class_init (GooroomFeedbackAppWindowClass *class)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  G_OBJECT_CLASS (class)->finalize = gooroom_feedback_app_window_finalize;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                  "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-app-window.ui");

  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackAppWindow,
                                                gfb_listbox);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackAppWindow,
                                                gfb_button_new_dialog);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackAppWindow,
                                                gfb_history_stack);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackAppWindow,
                                                gfb_history_none_box);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackAppWindow,
                                                gfb_history_scrolled_window);
}

GooroomFeedbackAppWindow *
gooroom_feedback_app_window_new (GtkApplication *app)
{
  return g_object_new (GOOROOM_TYPE_FEEDBACK_APP_WINDOW,
                       "application",
                       app,
                       NULL);
}
