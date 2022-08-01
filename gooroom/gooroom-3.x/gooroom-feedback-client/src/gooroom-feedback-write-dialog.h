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


#ifndef _GOOROOM_FEEDBACK_WRITE_DIALOG_H_
#define _GOOROOM_FEEDBACK_WRITE_DIALOG_H_

#include <gtk/gtk.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG            (gooroom_feedback_write_dialog_get_type ())
#define GOOROOM_FEEDBACK_WRITE_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG, GooroomFeedbackWriteDialog))
#define GOOROOM_FEEDBACK_WRITE_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG, GooroomFeedbackWriteDialogClass))
#define GOOROOM_IS_FEEDBACK_WRITE_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG))
#define GOOROOM_IS_FEEDBACK_WRITE_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG))
#define GOOROOM_FEEDBACK_WRITE_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GOOROOM_TYPE_FEEDBACK_WRITE_DIALOG, GooroomFeedbackWriteDialogClass))

typedef struct _GooroomFeedbackWriteDialog GooroomFeedbackWriteDialog;
typedef struct _GooroomFeedbackWriteDialogClass GooroomFeedbackWriteDialogClass;
typedef struct _GooroomFeedbackWriteDialogPrivate GooroomFeedbackWriteDialogPrivate;


struct _GooroomFeedbackWriteDialog {
  GtkDialog __parent__;

  GooroomFeedbackWriteDialogPrivate *priv;
};

struct _GooroomFeedbackWriteDialogClass {
  GtkDialogClass __parent_class__;
};


GType      gooroom_feedback_write_dialog_get_type (void) G_GNUC_CONST;

GtkWidget *gooroom_feedback_write_dialog_new      (GtkWindow *parent);


G_END_DECLS

#endif
