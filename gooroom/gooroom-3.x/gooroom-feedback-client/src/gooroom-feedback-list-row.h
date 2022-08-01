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

#ifndef __GOOROOM_FEEDBACK_LIST_ROW_H
#define __GOOROOM_FEEDBACK_LIST_ROW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GOOROOM_TYPE_FEEDBACK_LIST_ROW            (gooroom_feedback_list_row_get_type())
#define GOOROOM_FEEDBACK_LIST_ROW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GOOROOM_TYPE_FEEDBACK_LIST_ROW, GooroomFeedbackListRow))
#define GOOROOM_FEEDBACK_LIST_ROW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GOOROOM_TYPE_FEEDBACK_LIST_ROW, GooroomFeedbackListRowClass))
#define GOOROOM_IS_FEEDBACK_LIST_ROW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GOOROOM_TYPE_FEEDBACK_LIST_ROW))
#define GOOROOM_IS_FEEDBACK_LIST_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GOOROOM_TYPE_FEEDBACK_LIST_ROW))
#define GOOROOM_GET_FEEDBACK_LIST_ROW_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GOOROOM_TYPE_FEEDBACK_LIST_ROW, GooroomFeedbackListRowClass))

typedef struct _GooroomFeedbackListRow GooroomFeedbackListRow;
typedef struct _GooroomFeedbackListRowClass GooroomFeedbackListRowClass;
typedef struct _GooroomFeedbackListRowPrivate GooroomFeedbackListRowPrivate;


struct _GooroomFeedbackListRow {
  GtkListBoxRow __parent__;

  GooroomFeedbackListRowPrivate *priv;
};

struct _GooroomFeedbackListRowClass {
  GtkListBoxRowClass __parent_class__;
};

GType      gooroom_feedback_list_row_get_type (void) G_GNUC_CONST;
GtkWidget *gooroom_feedback_list_row_new      (void);
void       gooroom_feedback_list_row_expand   (GooroomFeedbackListRow *row);


G_END_DECLS

#endif /* __GOOROOM_FEEDBACK_LIST_ROW_H */
