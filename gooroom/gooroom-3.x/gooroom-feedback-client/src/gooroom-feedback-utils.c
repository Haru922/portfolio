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

#include <glib.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "gooroom-feedback-utils.h"

#define GOOROOM_FEEDBACK_LOG   "gooroom-feedback-client.log"



json_object *
JSON_OBJECT_GET (json_object *root_obj, const char *key)
{
  if (!root_obj) return NULL;

  json_object *ret_obj = NULL;

  json_object_object_get_ex (root_obj, key, &ret_obj);

  return ret_obj;
}

gchar *
get_feedback_log_path (void)
{
  return g_build_filename (g_get_user_data_dir (), GOOROOM_FEEDBACK_LOG, NULL);
}

int
gfb_get_os_info (char **release,
                 char **code_name)
{
  FILE *fp = NULL;
  int ret = -1;
  char *key = NULL;
  char *value = NULL;

  *release = NULL;
  *code_name = NULL;

  if (g_file_test (GOOROOM_OS_INFO, G_FILE_TEST_IS_REGULAR)
      && (fp = fopen (GOOROOM_OS_INFO, "r")))
  {
    char line[BUFSIZ] = { 0, };
    while (fgets (line, BUFSIZ, fp))
    {
      int len = strlen (line);
      if (0 < len && '\n' == line[len-1])
        line[len-1] = '\0';
      char *token = strtok (line, "=");
      while (token) {
        if (!key)
          key = token;
        else
          value = token;
        token = strtok (NULL, "=");
      }
      if (0 == strcmp ("RELEASE", key))
        *release = strdup (value);
      else if (0 == strcmp ("CODENAME", key))
        *code_name = strdup (value);
    }
  }
  
  if (fp)
    fclose (fp);
  fp = NULL;

  if (*release && *code_name)
    ret = 0;

  return ret;
}

int
gfb_post_request (const char *server_url,
                  const char *title,
                  const char *category,
                  const char *release,
                  const char *code_name,
                  const char *description)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist *list = NULL;
  long ret = GFB_RESPONSE_FAILURE;
  char *content = NULL;

  curl = curl_easy_init ();
  curl_slist_append (list, "Content-Type: application/json");

  if (curl) {
    curl_easy_setopt (curl, CURLOPT_URL, server_url);
    curl_easy_setopt (curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt (curl, CURLOPT_POST, 1L);
    char feedback[BUFSIZ] = { 0, };
    char *enc_title = curl_easy_escape (curl, title, strlen (title));
    char *enc_desc = curl_easy_escape (curl, description, strlen (description));
    snprintf (feedback, BUFSIZ,
              "title=%s&category=%s&release=%s&codename=%s&description=%s",
              enc_title, category, release, code_name, enc_desc);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, feedback);
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, 2L);
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt (curl, CURLOPT_CAINFO, "/etc/ssl/certs/GOOROOM_FEEDBACK_SERVER.crt");

    res = curl_easy_perform (curl);
    if (CURLE_OK == res) {
      curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &ret);
      curl_easy_getinfo (curl, CURLINFO_CONTENT_TYPE, &content); 
    } else
      ret = res;

    g_free (enc_title);
    g_free (enc_desc);

    curl_easy_cleanup (curl);
    curl_slist_free_all (list);
  }

  return ret;
}

const char *
gfb_status_code_to_string (int status_code)
{
  if (status_code < 100)
    return _("Internal Server Error.\nPlease check the network configuration.");
  else
    return _("External Server Error.\nPlease check the server configuration.");
}

//const char *
//gfb_server_error_message (int status_code)
//{
//  switch (status_code) {
//    case 100: return "Continue";
//    case 101: return "Switching Protocols";
//    case 102: return "Processing";
//    case 103: return "Early Hints";
//    case 200: return "OK";
//    case 201: return "Created";
//    case 202: return "Accepted";
//    case 203: return "Non Authoritative Info";
//    case 204: return "No Content";
//    case 205: return "Reset Content";
//    case 206: return "Partial Content";
//    case 207: return "Multi Status";
//    case 208: return "Already Reported";
//    case 226: return "IMUsed";
//    case 300: return "Multiple Choices";
//    case 301: return "Moved Permanently";
//    case 302: return "Found";
//    case 303: return "See Other";
//    case 304: return "Not Modified";
//    case 305: return "Use Proxy";
//    case 400: return "Bad Request";
//    case 401: return "Unauthorized";
//    case 402: return "Payment Required";
//    case 403: return "Forbidden";
//    case 404: return "Not Found";
//    case 405: return "Method Not Allowed";
//    case 406: return "Not Acceptable";
//    case 407: return "Proxy Auth Required";
//    case 408: return "Request Timeout";
//    case 409: return "Conflict";
//    case 410: return "Gone";
//    case 411: return "Length Required";
//    case 412: return "Precondition Failed";
//    case 413: return "Request Entity Too Large";
//    case 414: return "Request URI Too Long";
//    case 415: return "Unsupported Media Type";
//    case 416: return "Requested Range Not Satisfiable";
//    case 417: return "Expectation Failed";
//    case 418: return "Teapot";
//    case 421: return "Misdirected Request";
//    case 422: return "Unprocessable Entity";
//    case 423: return "Locked";
//    case 424: return "Failed Dependency";
//    case 425: return "Too Early";
//    case 426: return "Upgrade Required";
//    case 428: return "Precondition Required";
//    case 429: return "Too Many Requests";
//    case 431: return "Request Header Fields Too Large";
//    case 451: return "Unavailable For Legal Reasons";
//    case 500: return "Internal Server Error";
//    case 501: return "Not Implemented";
//    case 502: return "Bad Gateway";
//    case 503: return "Service Unavailable";
//    case 504: return "Gateway Timeout";
//    case 505: return "HTTP Version Not Supported";
//    case 506: return "Variant Also Negotiages";
//    case 507: return "Insufficient Storage";
//    case 508: return "Loop Detected";
//    case 510: return "Not Extended";
//    case 511: return "Network Authentication Required";
//    default: return "Unknown Status";
//  }
//}

gboolean
save_feedback_data (const gint64  time,
                    const gchar  *title,
                    const gchar  *category,
                    const gchar  *description,
                    const gchar  *response)
{
  gint rv = -1;
  gsize length = 0;
  gchar *contents = NULL;
  gchar *log_path = NULL;
  json_object *jso, *new_obj;
  
  log_path = get_feedback_log_path ();

  if (g_file_test (log_path, G_FILE_TEST_EXISTS))
    g_file_get_contents (log_path, &contents, &length, NULL);

  if (length > 0) {
    jso = json_object_from_file (log_path);
  } else {
    jso = json_object_new_array ();
  }

  // 파싱한 결과가 json_type_array 가 아닌경우 로그파일의
  // 내용을 삭제하고 json_object를 array 타입으로 재생성
  if (json_type_array != json_object_get_type (jso)) {
    g_warning ("Invalid log file format.");
    g_file_set_contents (log_path, "", -1, NULL);
    json_object_put (jso);
    jso = json_object_new_array ();
  }

  new_obj = json_object_new_object ();
  if (time >= 0)
    json_object_object_add (new_obj, "datetime", json_object_new_int64 (time));

  if (title)
    json_object_object_add (new_obj, "title", json_object_new_string (title));

  if (category)
    json_object_object_add (new_obj, "category", json_object_new_string (category));

  if (description)
    json_object_object_add (new_obj, "description", json_object_new_string (description));

  if (response)
    json_object_object_add (new_obj, "response", json_object_new_string (response));

  json_object_array_add (jso, new_obj);

  rv = json_object_to_file (log_path, jso);

  json_object_put (jso);

  g_clear_pointer (&log_path, g_free);

  return (-1 == rv) ? FALSE : TRUE;
}
