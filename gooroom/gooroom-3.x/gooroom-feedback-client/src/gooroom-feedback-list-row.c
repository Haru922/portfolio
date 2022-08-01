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

#include <stdio.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "gooroom-feedback-list-row.h"

#define GFB_RESOURCE_INIT       "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-client-init.svg"
#define GFB_RESOURCE_PROBLEM    "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-client-problem-symbolic.svg"
#define GFB_RESOURCE_SUGGESTION "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-client-suggestion.svg"


struct _GooroomFeedbackListRowPrivate {
  GtkWidget *category_image;
  GtkWidget *title_label;
  GtkWidget *datetime_label;
  GtkWidget *submit_status_label;
  GtkWidget *description_label;
  GtkWidget *description_revealer;

  gchar *datetime;
  gchar *category;
  gchar *title;
  gchar *description;
  gchar *response;
};

enum {
  PROP_0,
  PROP_CATEGORY,
  PROP_TITLE,
  PROP_DATETIME,
  PROP_DESCRIPTION,
  PROP_RESPONSE
};


G_DEFINE_TYPE_WITH_PRIVATE (GooroomFeedbackListRow, gooroom_feedback_list_row, GTK_TYPE_LIST_BOX_ROW)



static void
update_row (GooroomFeedbackListRow *row)
{
  const gchar *title;
  const gchar *datetime;
  const gchar *description;
  const gchar *response;
  GtkStyleContext *context;
  GooroomFeedbackListRowPrivate *priv = row->priv;

  title = (priv->title) ? priv->title : _("No Title");
  datetime = (priv->datetime) ? priv->datetime : _("Unknown");
  description = (priv->description) ? priv->description : _("No Description");
  response = (priv->response) ? priv->response : "FAILURE";

  if (priv->category) {
    if (g_str_equal (priv->category, "problem")) {
      gtk_image_set_from_icon_name (GTK_IMAGE (priv->category_image),
                                    "gooroom-feedback-client-problem-symbolic",
                                    GTK_ICON_SIZE_DIALOG);
    } else {
      gtk_image_set_from_icon_name (GTK_IMAGE (priv->category_image),
                                    "gooroom-feedback-client-suggestion-symbolic",
                                    GTK_ICON_SIZE_DIALOG);
    }
  }

  gtk_label_set_text (GTK_LABEL (priv->title_label), title);
  gtk_label_set_text (GTK_LABEL (priv->datetime_label), datetime);
  gtk_label_set_text (GTK_LABEL (priv->description_label), description);

  context = gtk_widget_get_style_context (priv->submit_status_label);
  gtk_style_context_remove_class (context, "success");
  gtk_style_context_remove_class (context, "failure");

  if (g_str_equal (response, "SUCCESS")) {
    gtk_style_context_add_class (context, "success");
    gtk_label_set_text (GTK_LABEL (priv->submit_status_label), _("Submit Success"));
  } else {
    gtk_style_context_add_class (context, "failure");
    gtk_label_set_text (GTK_LABEL (priv->submit_status_label), _("Submit Failure"));
  }
}

static void
gooroom_feedback_list_row_init (GooroomFeedbackListRow *self)
{
  GooroomFeedbackListRowPrivate *priv;

  priv = self->priv = gooroom_feedback_list_row_get_instance_private (self);

  priv->category = NULL;
  priv->title = NULL;
  priv->datetime = NULL;
  priv->description = NULL;
  priv->response = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
gooroom_feedback_list_row_finalize (GObject *object)
{
  GooroomFeedbackListRow *self = GOOROOM_FEEDBACK_LIST_ROW (object);
  GooroomFeedbackListRowPrivate *priv = self->priv;

  g_clear_pointer (&priv->category, g_free);
  g_clear_pointer (&priv->title, g_free);
  g_clear_pointer (&priv->datetime, g_free);
  g_clear_pointer (&priv->description, g_free);
  g_clear_pointer (&priv->response, g_free);

  G_OBJECT_CLASS(gooroom_feedback_list_row_parent_class)->finalize(object);
}

static void
gooroom_feedback_list_row_get_property (GObject        *object,
                                        guint           property_id,
                                        GValue         *value,
                                        GParamSpec     *pspec)
{
  GooroomFeedbackListRow *self = GOOROOM_FEEDBACK_LIST_ROW (object);
  GooroomFeedbackListRowPrivate *priv = self->priv;

  switch (property_id)
  {
    case PROP_CATEGORY:
      g_value_set_string (value, priv->category);
    break;

    case PROP_TITLE:
      g_value_set_string (value, priv->title);
    break;

    case PROP_DATETIME:
      g_value_set_string (value, priv->datetime);
    break;

    case PROP_DESCRIPTION:
      g_value_set_string (value, priv->description);
    break;

    case PROP_RESPONSE:
      g_value_set_string (value, priv->response);
    break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;
  }
}

static void
gooroom_feedback_list_row_set_property (GObject        *object,
                                        guint           property_id,
                                        const GValue   *value,
                                        GParamSpec     *pspec)
{
  GooroomFeedbackListRow *self = GOOROOM_FEEDBACK_LIST_ROW (object);
  GooroomFeedbackListRowPrivate *priv = self->priv;

  switch (property_id)
  {
    case PROP_CATEGORY:
      g_clear_pointer (&priv->category, g_free);
      priv->category = g_value_dup_string (value);
      update_row (self);
    break;

    case PROP_TITLE:
      g_clear_pointer (&priv->title, g_free);
      priv->title = g_value_dup_string (value);
      update_row (self);
    break;

    case PROP_DATETIME:
      g_clear_pointer (&priv->datetime, g_free);
      priv->datetime = g_value_dup_string (value);
      update_row (self);
    break;

    case PROP_DESCRIPTION:
      g_clear_pointer (&priv->description, g_free);
      priv->description = g_value_dup_string (value);
      update_row (self);
    break;

    case PROP_RESPONSE:
      g_clear_pointer (&priv->response, g_free);
      priv->response = g_value_dup_string (value);
      update_row (self);
    break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;
  }
}

static void
gooroom_feedback_list_row_class_init (GooroomFeedbackListRowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

//  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  object_class->finalize = gooroom_feedback_list_row_finalize;
  object_class->get_property = gooroom_feedback_list_row_get_property;
  object_class->set_property = gooroom_feedback_list_row_set_property;

  g_object_class_install_property (object_class, PROP_DATETIME,
                                   g_param_spec_string ("datetime", NULL,
                                   "DateTime", NULL, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_TITLE,
                                   g_param_spec_string ("title", NULL,
                                   "Title", NULL, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_CATEGORY,
                                   g_param_spec_string ("category", NULL,
                                   "Category", NULL, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_DESCRIPTION,
                                   g_param_spec_string ("description", NULL,
                                   "Description", NULL, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_RESPONSE,
                                   g_param_spec_string ("response", NULL,
                                   "Response", NULL, G_PARAM_READWRITE));

  gtk_widget_class_set_template_from_resource (widget_class,
                  "/kr/gooroom/gooroom-feedback-client/gooroom-feedback-list-row.ui");

  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, category_image);
  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, title_label);
  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, datetime_label);
  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, submit_status_label);
  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, description_label);
  gtk_widget_class_bind_template_child_private (widget_class, GooroomFeedbackListRow, description_revealer);
}

GtkWidget *
gooroom_feedback_list_row_new (void)
{
  return g_object_new (GOOROOM_TYPE_FEEDBACK_LIST_ROW, NULL);
}

void
gooroom_feedback_list_row_expand (GooroomFeedbackListRow *row)
{
  g_return_if_fail (GOOROOM_IS_FEEDBACK_LIST_ROW (row));

  gboolean expand;
  GooroomFeedbackListRowPrivate *priv = row->priv;

  expand = !gtk_revealer_get_reveal_child (GTK_REVEALER (priv->description_revealer));
//  gtk_widget_set_visible (priv->description_revealer, expand);
  gtk_revealer_set_reveal_child (GTK_REVEALER (priv->description_revealer), expand);
}
