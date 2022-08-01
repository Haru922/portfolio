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


#ifndef _GOOROOM_FEEDBACK_CLIENT_UTILS_H_
#define _GOOROOM_FEEDBACK_CLIENT_UTILS_H_

#include <stdio.h>
#include <string.h>
#include <glib-object.h>
#include <json-c/json.h>


#define GFB_RESPONSE_SUCCESS 201
#define GFB_RESPONSE_FAILURE   0

#define GOOROOM_OS_INFO "/etc/gooroom/info"

int         gfb_get_os_info           (char **release, char **code_name);
int         gfb_post_request          (const char *server_url,
                                       const char *title,
                                       const char *category,
                                       const char *release,
                                       const char *code_name,
                                       const char *description);

const char *gfb_status_code_to_string (int status_code);

json_object *JSON_OBJECT_GET (json_object *root_obj, const char *key);

gchar *     get_feedback_log_path     (void);

gboolean    save_feedback_data        (const gint64  time,
                                       const gchar  *title,
                                       const gchar  *category,
                                       const gchar  *description,
                                       const gchar  *response);

#endif
