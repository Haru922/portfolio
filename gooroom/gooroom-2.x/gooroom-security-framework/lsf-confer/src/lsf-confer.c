#include "lsf-confer.h"
#include <glib/gprintf.h>

Confer *
lsf_get_confer (char *config_path,
                int   reload_flag)
{
  GError *error = NULL;

  if (confer != NULL)
  {
    if (reload_flag == CONFER_RELOAD_TRUE)
    {
      g_key_file_free (confer->key_file);
    }
    else
    {
      if ((g_key_file_get_start_group (confer->key_file)) != NULL)
      {
        return confer;
      }
      g_key_file_free (confer->key_file);
    }
  }

  if (config_path == NULL)
  {
    config_path = LSF_CONFER_CONFIG;
  }

  confer = (Confer *) malloc (sizeof (Confer));

  confer->key_file = g_key_file_new ();

  if (!g_key_file_load_from_file (confer->key_file, config_path, G_KEY_FILE_NONE, &error))
  {
    if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)
        || !g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_NOT_FOUND))
    {
      g_print ("Error loading key file: %s\n", error->message);
    }
    g_error_free (error);

    return NULL; 
  }
  return confer;
}

void
lsf_confer_free (void)
{
  g_key_file_free (confer->key_file);
  free (confer);
}

static void
confer_util_error_print (GError *error)
{
  if (error != NULL) {
    if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_GROUP_NOT_FOUND))
    {
      g_print ("Error finding section in key file: %s\n", error->message);
    }
    else if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
    {
      g_print ("Error finding key in key file: %s\n", error->message);
    }
    else if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_INVALID_VALUE))
    {
      g_print ("Invalid value found: %s\n", error->message);
    }
    g_error_free (error);
  }
}

gchar *
lsf_confer_get_str (char *section,
                    char *key)
{
  GError *error = NULL;
  gchar  *value = NULL;

  if (confer->key_file == NULL) 
  {
    g_error ("Cannot read the key file.");
    return NULL;
  }

  value = g_key_file_get_string (confer->key_file, section, key, &error);

  if (value == NULL)
  {
    if (error != NULL)
    {
      confer_util_error_print (error);
    }
    else
    {
      g_warning ("No Value for the Section:[%s]\nKey:%s.\n", section, key);
    }
    return NULL;
  }

  return value;
}

gint
lsf_confer_get_integer (char *section,
                        char *key)
{
  GError *error = NULL;
  gint    value = 0;

  if (confer->key_file == NULL)
  {
    g_print ("Cannot read the key file..");
    return value;
  }

  value = g_key_file_get_integer (confer->key_file, section, key, &error);

  if (!value)
  {
    if (error != NULL)
    {
      confer_util_error_print (error);
    }
    else
    {
      g_warning ("No Value for the Section:[%s]\nKey:%s.\n", section, key);
    }
  }
  return value;
}

void
lsf_confer_set_str (char *section,
                    char *key,
                    char *value)
{
  g_key_file_set_string (confer->key_file,
                         section,
                         key,
                         value);
}

gboolean
lsf_confer_save ()
{
  GError *error = NULL;

  if (!g_key_file_save_to_file (confer->key_file, LSF_CONFER_CONFIG, &error))
  {
    g_print ("Cannot save the contents: %s\n", error->message);
    g_error_free (error);
    return FALSE;
  }
  return TRUE;
}

gboolean
lsf_confer_remove_section (char *group_name)
{
  GError *error = NULL;

  if (!g_key_file_remove_group (confer->key_file, group_name, &error))
  {
    g_print ("Cannot remove the section[%s]: %s\n", group_name, error->message);
    g_error_free (error);
    return FALSE;
  }
  return TRUE;
}

gboolean
lsf_confer_remove_key (char *group_name,
                       char *key_name)
{
  GError *error = NULL;

  if (!g_key_file_remove_key (confer->key_file, group_name, key_name, &error))
  {
    g_print ("Cannot remove the key[%s / %s]: %s\n", group_name, key_name, error->message);
    g_error_free (error);
    return FALSE;
  }
  return TRUE;
}
