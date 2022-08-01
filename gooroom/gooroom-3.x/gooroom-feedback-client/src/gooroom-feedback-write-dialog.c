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

#include "gooroom-feedback-utils.h"
#include "gooroom-feedback-write-dialog.h"


struct _GooroomFeedbackWriteDialogPrivate
{
  GtkWidget *title_entry;
  GtkWidget *category_problem_radio;
  GtkWidget *category_suggestion_radio;
  GtkWidget *description_buffer;
  GtkWidget *submit_button;
  GtkWidget *cancel_button;

  gchar *server_url;
};

G_DEFINE_TYPE_WITH_PRIVATE (GooroomFeedbackWriteDialog, gooroom_feedback_write_dialog, GTK_TYPE_DIALOG);



static void
show_result_message (GtkWindow   *parent,
                     const gchar *message)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (parent,
                                   0,
                                   GTK_MESSAGE_INFO,
                                   GTK_BUTTONS_CLOSE,
                                   message, NULL);
  gtk_window_set_title (GTK_WINDOW (dialog), _("Gooroom Feedback Submit"));
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void
cancel_button_clicked_cb (GtkButton *widget,
                          gpointer   user_data)
{
  gtk_dialog_response (GTK_DIALOG (user_data), GTK_RESPONSE_CANCEL);
}

static void
submit_button_clicked_cb (GtkButton *widget,
                          gpointer   user_data)
{
  gint64 utime = 0;
  gboolean exit = FALSE;
  gchar *release = NULL;
  gchar *time_str = NULL;
  gchar *code_name = NULL;
  gchar *description = NULL;
  gchar *full_description = NULL;
  gchar *response_msg = NULL;
  const gchar *title = NULL;
  const gchar *category = NULL;
  const gchar *server_response = NULL;
  GDateTime *dt = NULL;
  GtkTextIter start_iter, end_iter;

  GooroomFeedbackWriteDialog *dialog = GOOROOM_FEEDBACK_WRITE_DIALOG (user_data);
  GooroomFeedbackWriteDialogPrivate *priv = dialog->priv;

  title = gtk_entry_get_text (GTK_ENTRY (priv->title_entry));

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (priv->category_problem_radio)))
    category = "problem";
  else
    category = "suggestion";

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (priv->description_buffer), &start_iter);
  gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (priv->description_buffer), &end_iter);
  description = gtk_text_buffer_get_text (GTK_TEXT_BUFFER (priv->description_buffer),
                                          &start_iter,
                                          &end_iter,
                                          FALSE);

  gfb_get_os_info (&release, &code_name);


  dt = g_date_time_new_now_local ();
  utime = g_date_time_to_unix (dt);
  g_date_time_unref (dt);

  if (strlen (title) && strlen (description))
  {
    long response = GFB_RESPONSE_FAILURE;
	full_description = g_strdup_printf ("RELEASE = %s\nCODENAME = %s\n\n%s\n", release, code_name, description);
    response = gfb_post_request (priv->server_url,
                                 title,
                                 category,
                                 release,
                                 code_name,
                                 full_description);

    if (GFB_RESPONSE_SUCCESS == response)
    {
      server_response = "SUCCESS";
      response_msg = g_strdup (_("\nThanks for taking the time to give us feedback.\n"));
    }
    else
    {
      server_response = "FAILURE";
	  response_msg = g_strdup_printf ("%s [%ld]", gfb_status_code_to_string (response), response);
    }

    if (!save_feedback_data (utime, title, category, description, server_response)) {
    }

    exit = TRUE;
  }
  else
    response_msg = g_strdup (_("\nPlease provide us more detailed information of your feedback.\n"));

  g_clear_pointer (&release, g_free);
  g_clear_pointer (&time_str, g_free);
  g_clear_pointer (&code_name, g_free);
  g_clear_pointer (&description, g_free);
  g_clear_pointer (&full_description, g_free);

  show_result_message (GTK_WINDOW (user_data), response_msg);

  if (exit)
    gtk_dialog_response (GTK_DIALOG (user_data), GTK_RESPONSE_OK);

  g_clear_pointer (&response_msg, g_free);
}

static void
gooroom_feedback_write_dialog_finalize (GObject *object)
{
  GooroomFeedbackWriteDialogPrivate *priv = GOOROOM_FEEDBACK_WRITE_DIALOG (object)->priv;

  g_clear_pointer (&priv->server_url, g_free);

  G_OBJECT_CLASS (gooroom_feedback_write_dialog_parent_class)->finalize (object);
}

static void
gooroom_feedback_write_dialog_init (GooroomFeedbackWriteDialog *self)
{
  GSettings *settings = NULL;
  GooroomFeedbackWriteDialogPrivate *priv = NULL;

  priv = self->priv = gooroom_feedback_write_dialog_get_instance_private (self);

  gtk_widget_init_template (GTK_WIDGET (self));

//  GtkStyleContext *context;
//  context = gtk_widget_get_style_context (priv->gfb_button_submit);
//  gtk_style_context_add_class (context, "suggested-action");

  settings = g_settings_new ("kr.gooroom.FeedbackClient");
  priv->server_url = g_settings_get_string (settings, "server-url");

  g_clear_object (&settings);

  g_signal_connect (priv->submit_button,
                    "clicked",
                    G_CALLBACK (submit_button_clicked_cb),
                    self);

  g_signal_connect (priv->cancel_button,
                    "clicked",
                    G_CALLBACK (cancel_button_clicked_cb),
                    self);
}

static void
gooroom_feedback_write_dialog_class_init (GooroomFeedbackWriteDialogClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  gobject_class->finalize = gooroom_feedback_write_dialog_finalize;

  gtk_widget_class_set_template_from_resource (widget_class,
                           "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-write-dialog.ui");

  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                title_entry);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                category_problem_radio);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                category_suggestion_radio);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                description_buffer);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                submit_button);
  gtk_widget_class_bind_template_child_private (widget_class,
                                                GooroomFeedbackWriteDialog,
                                                cancel_button);
}

GtkWidget *
gooroom_feedback_write_dialog_new (GtkWindow *parent)
{
  GObject *object;

  object = g_object_new (GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG,
                         "use-header-bar", TRUE,
                         "transient-for", parent,
                         NULL);

//  if (parent != NULL)
//    gtk_window_set_transient_for (GTK_WINDOW (object), parent);

  return GTK_WIDGET (object);
}
