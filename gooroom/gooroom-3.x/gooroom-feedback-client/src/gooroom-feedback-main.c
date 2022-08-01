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

#include "gooroom-feedback-app-window.h"


static GooroomFeedbackAppWindow *win;

static void
on_app_startup_cb (GtkApplication *app, gpointer data)
{
  win = gooroom_feedback_app_window_new (app);
}

static void
on_app_activate_cb (GtkApplication *app, gpointer data)
{
  GtkCssProvider *provider;
  provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_resource (provider,
                                 "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-client.css");
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
                                             GTK_STYLE_PROVIDER (provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref (provider);

  gtk_window_present (GTK_WINDOW (win));
}

int
main (int   argc,
      char *argv[])
{
  int status;
  GtkApplication *app;

  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  app = gtk_application_new ("kr.gooroom.gooroom-feedback-client", G_APPLICATION_FLAGS_NONE);

  g_signal_connect (app, "activate", G_CALLBACK (on_app_activate_cb), NULL);
  g_signal_connect (app, "startup", G_CALLBACK (on_app_startup_cb), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);

  g_object_unref (app);

  return status;
}
