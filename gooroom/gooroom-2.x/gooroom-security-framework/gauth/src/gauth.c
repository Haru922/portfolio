#include "lsf-util-common.h"
#include "lsf-auth-common.h"
#include "lsf-define-common.h"
#include "lsf/lsf-auth.h"
#include "gauth.h"
#include "gauth-func.h"

static void         method_call                     (GDBusConnection *, const gchar *, const gchar *, const gchar *,
                                                     const gchar *, GVariant *, GDBusMethodInvocation *, gpointer);
static void         dbus_thread                     (gpointer, gpointer);
static void         do_task                         (dbus_msg_t *);

static void         signal_handler                  (int);
static void         on_bus_acquired                 (GDBusConnection *, const gchar *, gpointer);
static void         on_name_acquired                (GDBusConnection *, const gchar *, gpointer);
static void         on_name_lost                    (GDBusConnection *, const gchar *, gpointer);

static int          gauth_create                    (void);
static int          gauth_initialize                (void);
static int          gauth_connect                   (void);
static int          gauth_load                      (void);
static int          gauth_reload                    (void);
static int          gauth_run                       (void);
static int          gauth_destroy                   (void);

static void         gauth_message_init              (dbus_msg_t *);
static int          gauth_message_parse             (dbus_msg_t *);
static int          gauth_message_verify_sender     (dbus_msg_t *);
static int          gauth_message_verify_glyph      (dbus_msg_t *);
static int          gauth_message_verify_function   (dbus_msg_t *);
static int          gauth_message_process_auth      (dbus_msg_t *);
static int          gauth_message_process_reauth    (dbus_msg_t *);
static int          gauth_message_process_reload    (dbus_msg_t *);
static int          gauth_message_invocation_return (dbus_msg_t *);
static int          gauth_message_invocation_error  (dbus_msg_t *);
static int          gauth_message_glyph_return      (dbus_msg_t *);
static int          gauth_message_glyph_error       (dbus_msg_t *);

static const char * gauth_state_get_string          (int);
static const char * gauth_message_state_get_string  (int);
static const char * gauth_error_get_string          (int);

static int          gauth_app_info_set              (const char **);
static void         gauth_app_info_print            (void);
static app_info_t * gauth_app_info_find             (int, const char *);
static void         gauth_app_info_delete           (void);

GAuth *self;

static void
method_call (GDBusConnection *connection,
             const gchar *sender,
             const gchar *object_path,
             const gchar *interface_name,
             const gchar *method_name,
             GVariant *parameters,
             GDBusMethodInvocation *invocation,
             gpointer user_data)
{
  gchar *string_json = NULL;
  dbus_msg_t *msg = (dbus_msg_t *) calloc (1, sizeof (dbus_msg_t));

  g_variant_get (parameters, "(&s)", &string_json);

  msg->invocation = invocation;
  msg->in_json = string_json;
  msg->method_name = method_name;
  msg->sender = sender;
  gauth_message_init (msg);
   
  if (!g_thread_pool_push (self->pool, (gpointer) msg, NULL)) {
    msg->out_glyph = GLYPH_ERR;
    msg->error_code = GAUTH_ERROR_THREAD_NOT_AVAILABLE;
    gauth_message_invocation_error (msg);
  }
}

static void
dbus_thread (gpointer data, gpointer user_data) {
  dbus_msg_t *msg;

  if (data) {
    msg = (dbus_msg_t *) data;
    if (strcmp (msg->method_name, "do_task") == 0)
      do_task (msg);
    else {
      msg->out_glyph = GLYPH_ERR;
      msg->error_code = GAUTH_ERROR_UNKNOWN_METHOD;
      gauth_message_invocation_error (msg);
    }
  }
}

static void
do_task (dbus_msg_t *msg) {
  while (msg->state != GAUTH_MESSAGE_STATE_RETURNED)
    self->handle_message[msg->state](msg);

  fprintf (stdout, "GAUTH MESSAGE STATE: %s\n", gauth_message_state_get_string (msg->state));

  if (msg) {
    lsf_free (msg->in_glyph);
    lsf_free (msg->function);
    lsf_free (msg->letter);
    lsf_free (msg->out_json);
    lsf_free (msg);
  }
}

static void
signal_handler (int signum)
{
  self->state = GAUTH_STATE_DEAD;
  self->gauth_action[self->state]();
}

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  static const GDBusInterfaceVTable iface_vtable = { method_call };

  self->bus_reg_id = g_dbus_connection_register_object (connection,
                                                        GAUTH_DBUS_OBJ,
                                                        self->node_info->interfaces[0],
                                                        &iface_vtable,
                                                        NULL,  
                                                        NULL, 
                                                        NULL); 
  if (self->bus_reg_id > 0)
    fprintf (stdout, "=== BUS ACQUIRED ===\n");
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  fprintf (stdout, "=== NAME ACQUIRED ===\n");
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  fprintf (stdout, "=== NAME LOST ===\n");
  self->state = GAUTH_STATE_DEAD;
  fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));

  self->gauth_action[self->state]();
}

static int
gauth_create (void) {
  self = (GAuth *) calloc (1, sizeof (GAuth));

  if (!self)
    return GAUTH_ERROR_MEMORY_ALLOCATION;

  self->logger = lsf_get_logger ("gauth");
  if (!self->logger)
    return GAUTH_ERROR_CANNOT_GET_LOGGER;

  self->gauth_action[GAUTH_STATE_CREATED] = &gauth_initialize;
  self->gauth_action[GAUTH_STATE_INITIALIZED] = &gauth_load;
  self->gauth_action[GAUTH_STATE_LOADED] = &gauth_connect;
  self->gauth_action[GAUTH_STATE_CONNECTED] = &gauth_run;
  self->gauth_action[GAUTH_STATE_DEAD] = &gauth_destroy;

  self->handle_message[GAUTH_MESSAGE_STATE_RECEIVED] = &gauth_message_verify_sender;
  self->handle_message[GAUTH_MESSAGE_STATE_SENDER_VERIFIED] = &gauth_message_parse;
  self->handle_message[GAUTH_MESSAGE_STATE_PARSED] = &gauth_message_verify_glyph;
  self->handle_message[GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_NORMAL] = &gauth_message_verify_function;
  self->handle_message[GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_REAUTH] = &gauth_message_process_reauth;
  self->handle_message[GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_RELOAD] = &gauth_message_process_reload;
  self->handle_message[GAUTH_MESSAGE_STATE_FUNCTION_VERIFIED] = &gauth_message_process_auth;
  self->handle_message[GAUTH_MESSAGE_STATE_GLYPH_PROCESSED] = &gauth_message_glyph_return;
  self->handle_message[GAUTH_MESSAGE_STATE_FUNCTION_PROCESSED] = &gauth_message_invocation_return;
  self->handle_message[GAUTH_MESSAGE_STATE_MESSAGE_ERROR] = &gauth_message_invocation_error;
  self->handle_message[GAUTH_MESSAGE_STATE_GLYPH_PROCESS_ERROR] = &gauth_message_glyph_error;
  self->handle_message[GAUTH_MESSAGE_STATE_FUNCTION_PROCESS_ERROR] = &gauth_message_invocation_error;

  self->state = GAUTH_STATE_CREATED;
  fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));

  return GAUTH_RETURN_SUCCESS;
}

static int
gauth_initialize (void) {
  const char GAUTH_INTROSPECTION_XML[] =
  "<node>"
  "  <interface name='"GAUTH_DBUS_IFACE"'>"
  "    <method name='do_task'>"
  "      <arg type='s' name='json_string' direction='in'/>"
  "      <arg type='s' name='json_string' direction='out'/>"
  "    </method>"
  "  </interface>"
  "</node>"; 
  self->head = NULL;
  self->tail = NULL;
  self->symm_key = NULL;
  self->loop = g_main_loop_new (NULL, FALSE);
  self->owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                   GAUTH_DBUS_NAME,
                                   G_BUS_NAME_OWNER_FLAGS_NONE,
                                   on_bus_acquired,
                                   on_name_acquired,
                                   on_name_lost,
                                   NULL,
                                   NULL);
  self->pool = g_thread_pool_new (dbus_thread,
                                  NULL,
                                  MAX_THREAD,
                                  FALSE,
                                  NULL);
  self->node_info = g_dbus_node_info_new_for_xml (GAUTH_INTROSPECTION_XML, NULL);

  signal (SIGINT, signal_handler);
  signal (SIGTERM, signal_handler);

  self->state = GAUTH_STATE_INITIALIZED;
  fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));

  return GAUTH_RETURN_SUCCESS;
}

static int
gauth_connect (void) {
  int ret = GAUTH_RETURN_SUCCESS;
  char *prime = NULL;
  char *generator = NULL;
  lsf_user_data_t data;

  memset (&data, 0, sizeof (lsf_user_data_t));

  if (lsf_connect (&data, &prime, &generator)) {
    if (lsf_dh_key_request (&data, prime, generator)) {
      if (self->symm_key)
        lsf_free (self->symm_key);
      self->symm_key = strdup (data.symm_key);
    } else
      ret = GAUTH_ERROR_LSF_DH_KEY_REQUEST;
  } else
    ret = GAUTH_ERROR_LSF_CONNECT;

  lsf_free (prime);
  lsf_free (generator);

  if (ret == GAUTH_RETURN_SUCCESS && self->state != GAUTH_STATE_RUNNING) {
    self->state = GAUTH_STATE_CONNECTED;
    fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));
  }
   
  return ret;
}

static int
gauth_load (void) {
  json_object *root_obj = NULL;
  json_object *policy_obj = NULL;
  json_object *module_obj = NULL;
  json_object *field_iter_obj = NULL;
  char *policy_contents;
  const char *app_info[GAUTH_APP_POLICY_NUM];
  int module_num = 0;
  int i, j;
  int ret = GAUTH_RETURN_SUCCESS;
  FILE *fp = fopen (LSF_POLICY_PATH, "r");
  struct stat st;
  const char *policy_field[GAUTH_APP_POLICY_NUM] = { "abs_path",
                                                     "private_key",
                                                     "display_name",
                                                     "exp",
                                                     "dbus_name" };
  
  if (!fp)
    return GAUTH_ERROR_FILE_OPEN;

  fstat (fileno (fp), &st);
  policy_contents = (char*) calloc (1, st.st_size + 1);
  fread (policy_contents, st.st_size, 1, fp);
  policy_contents[st.st_size] = '\0';
  fclose (fp);

  root_obj = json_tokener_parse (policy_contents);
  lsf_free (policy_contents);
  if (!root_obj) 
    return GAUTH_ERROR_JSON_PARSING;

  if (!json_object_object_get_ex (root_obj, "policy", &policy_obj)) {
    json_object_put (root_obj);
    return GAUTH_ERROR_JSON_OBJECT_GET;
  }

  module_num = json_object_array_length (policy_obj);
  if (module_num <= 0) {
    ret = GAUTH_ERROR_JSON_ARRAY_LENGTH;
    goto GAUTH_LOAD_END;
  }

  for (i = 0; i < module_num; i++) {
    if (ret) goto GAUTH_LOAD_END;
    module_obj = json_object_array_get_idx (policy_obj, i);
    if (!module_obj) {
      ret = GAUTH_ERROR_JSON_ARRAY_GET;
      goto GAUTH_LOAD_END;
    }
    for (j = 0; j < GAUTH_APP_POLICY_NUM; j++) {
      if (json_object_object_get_ex (module_obj, policy_field[j], &field_iter_obj))
        app_info[j] = json_object_get_string (field_iter_obj);
      else {
        ret = GAUTH_ERROR_JSON_OBJECT_GET;
        goto GAUTH_LOAD_END;
      }
    }
    ret = gauth_app_info_set (app_info);
  }
  gauth_app_info_print();

GAUTH_LOAD_END:
  if (root_obj) 
    json_object_put (root_obj);

  if (ret == GAUTH_RETURN_SUCCESS && self->state != GAUTH_STATE_RUNNING) {
    self->state = GAUTH_STATE_LOADED;
    fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));
  }

  return ret;
}

static int
gauth_reload (void) {
  gauth_app_info_delete ();
  return gauth_load ();
}

static int
gauth_run (void) {
  if (!self->loop)
    return GAUTH_RETURN_FAILURE;

  self->state = GAUTH_STATE_RUNNING;
  fprintf (stdout, "GAUTH STATE: %s\n", gauth_state_get_string (self->state));

  g_main_loop_run (self->loop);

  return GAUTH_RETURN_SUCCESS;
}

static int
gauth_destroy (void) {
  if (self->loop && g_main_loop_is_running (self->loop)) {
    g_main_loop_quit (self->loop);
    g_main_loop_unref (self->loop);
    self->loop = NULL;
  }
  if (self->owner_id > 0) {
    g_bus_unown_name (self->owner_id);
    self->owner_id = 0;
  }
  if (self->node_info) {
    g_dbus_node_info_unref (self->node_info);
    self->node_info = NULL;
  }
  if (self->pool) {
    g_thread_pool_free (self->pool, true, false);
    self->pool = NULL;
  }

  if (self->logger) {
    lsf_logger_free (self->logger);
    self->logger = NULL;
  }

  return GAUTH_RETURN_SUCCESS;
}

static void
gauth_message_init (dbus_msg_t *msg) {
  msg->out_json = NULL;
  msg->function = NULL;
  msg->in_glyph = NULL;
  msg->out_glyph = GLYPH_NORMAL;
  msg->error_code = GAUTH_ERROR_NONE;
  msg->state = GAUTH_MESSAGE_STATE_RECEIVED;
  msg->target_app = NULL;
}

static int
gauth_message_parse (dbus_msg_t *msg) {
  json_object *root_obj = NULL;
  json_object *iter_obj = NULL;
  int ret = GAUTH_RETURN_SUCCESS;

  root_obj = json_tokener_parse (msg->in_json);
  if (!root_obj
      || !json_object_object_get_ex (root_obj, "seal", &iter_obj)
      || !json_object_object_get_ex (iter_obj, "glyph", &iter_obj)) {
    ret = GAUTH_ERROR_JSON_PARSING;
    goto GAUTH_MESSAGE_PARSE_END;
  }
  msg->in_glyph = strdup (json_object_get_string (iter_obj));

  if (!json_object_object_get_ex (root_obj, "letter", &iter_obj)) goto GAUTH_MESSAGE_PARSE_END;
  msg->letter = strdup (json_object_get_string (iter_obj));

GAUTH_MESSAGE_PARSE_END:
  if (ret == GAUTH_RETURN_SUCCESS) 
    msg->state = GAUTH_MESSAGE_STATE_PARSED;
  else {
    msg->out_glyph = GLYPH_ERR;
    msg->state = GAUTH_MESSAGE_STATE_MESSAGE_ERROR;
    msg->error_code = ret;
  }
  if (root_obj)
    json_object_put (root_obj);

  return ret;
}

static int
gauth_message_verify_sender (dbus_msg_t *msg) {
  GDBusConnection *conn = NULL;
  GDBusProxy *proxy = NULL;
  GVariant *pidvar = NULL;
  app_info_t *app = NULL;
  guint pid = 0;
  int app_found = FALSE;
  int ret = GAUTH_RETURN_SUCCESS;

  conn = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, NULL);
  if (!conn) {
    ret = GAUTH_ERROR_DBUS_GET;
    goto GAUTH_MESSAGE_VERIFY_SENDER_END;
  }

  proxy = g_dbus_proxy_new_sync (conn,
                                 G_DBUS_PROXY_FLAGS_NONE,
                                 NULL,
                                 "org.freedesktop.DBus",
                                 "/org/freedesktop/DBus",
                                 "org.freedesktop.DBus",
                                 NULL,
                                 NULL);
  if (!proxy) {
    ret = GAUTH_ERROR_DBUS_PROXY_NEW;
    goto GAUTH_MESSAGE_VERIFY_SENDER_END;
  }

  pidvar = g_dbus_proxy_call_sync (proxy,
                                   "org.freedesktop.DBus.GetConnectionUnixProcessID",
                                   g_variant_new ("(s)", msg->sender),
                                   G_DBUS_CALL_FLAGS_NONE,
                                   500,
                                   NULL,
                                   NULL);
  if (!pidvar) {
    ret = GAUTH_ERROR_DBUS_PROXY_CALL;
    goto GAUTH_MESSAGE_VERIFY_SENDER_END;
  }

  g_variant_get (pidvar, "(u)", &pid);
  if (!pid) {
    ret = GAUTH_ERROR_PID_NOT_FOUND;
    goto GAUTH_MESSAGE_VERIFY_SENDER_END;
  }

  for (app = self->head; app; app = app->next)
    if (lsf_validate_pid_path (pid, app->app_policy_field[GAUTH_APP_POLICY_ABS_PATH])) {
      app_found = TRUE;
      break;
    }

  if (!app_found)
    ret = GAUTH_ERROR_APP_NOT_FOUND;

GAUTH_MESSAGE_VERIFY_SENDER_END:
  if (ret == GAUTH_RETURN_SUCCESS) 
    msg->state = GAUTH_MESSAGE_STATE_SENDER_VERIFIED;
  else {
    msg->out_glyph = GLYPH_ERR;
    msg->state = GAUTH_MESSAGE_STATE_MESSAGE_ERROR;
    msg->error_code = ret;
  }
  if (proxy) 
    g_object_unref (proxy);
  if (conn)
    g_object_unref (conn);
  if (pidvar) 
    g_variant_unref (pidvar);

  return ret;
}

static int
gauth_message_verify_glyph (dbus_msg_t *msg) {
  int ret = GAUTH_RETURN_SUCCESS;

  if (strcmp (msg->in_glyph, GLYPH_NORMAL)) {
    if (strcmp (msg->in_glyph, GLYPH_RELOAD) == 0) {
      msg->state = GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_RELOAD;
      GLOGI (self->logger, "[MESSAGE IN: (glyph) %s]", msg->in_glyph);
    } else if (strcmp (msg->in_glyph, GLYPH_WHO) == 0) {
      msg->state = GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_REAUTH;
      GLOGI (self->logger, "[MESSAGE IN: (glyph) %s]", msg->in_glyph);
    } else {
      GLOGW (self->logger, "[MESSAGE IN: (glyph) %s]", msg->in_glyph);
      msg->out_glyph = GLYPH_ERR;
      msg->state = GAUTH_MESSAGE_STATE_MESSAGE_ERROR;
      ret = GAUTH_ERROR_UNKNOWN_GLYPH;
      msg->error_code = ret;
    }
  } else 
    msg->state = GAUTH_MESSAGE_STATE_GLYPH_VERIFIED_NORMAL;

  return ret;
}

static int
gauth_message_verify_function (dbus_msg_t *msg) {
  json_object *letter_obj = NULL;
  json_object *iter_obj = NULL;
  int ret = GAUTH_RETURN_SUCCESS;
  const char *ghub_abs_path;

  if (!msg->letter) {
    ret = GAUTH_ERROR_EMPTY_LETTER;
    msg->error_code = ret;
    msg->out_glyph = GLYPH_ERR;
    msg->state = GAUTH_MESSAGE_STATE_MESSAGE_ERROR;
    return ret;
  }
  msg->letter = (gchar *) lsf_decode_base64_and_decrypt_aes (msg->letter, self->symm_key);
  
  letter_obj = json_tokener_parse (msg->letter);
  if (letter_obj
      && json_object_object_get_ex (letter_obj, "function", &iter_obj))
    msg->function = strdup (json_object_get_string (iter_obj));
  
  if (json_object_object_get_ex (letter_obj, "params", &iter_obj)
      && json_object_object_get_ex (iter_obj, "ghub_abspath", &iter_obj)) {
    ghub_abs_path = json_object_get_string (iter_obj);
    msg->target_app = gauth_app_info_find (GAUTH_FIND_APP_BY_ABS_PATH, ghub_abs_path);
  }

  if (msg->target_app) {
    if (strcmp (msg->function, "auth") == 0) {
      msg->state = GAUTH_MESSAGE_STATE_FUNCTION_VERIFIED;
      GLOGI (self->logger, "[MESSAGE IN: (glyph) %s ::: (function) %s]", msg->in_glyph, msg->function);
    } else {
      ret = GAUTH_ERROR_UNKNOWN_FUNCTION;
      GLOGW (self->logger, "[MESSAGE IN: (glyph) %s ::: (function) %s]", msg->in_glyph, msg->function);
    }
  }
  else
    ret = GAUTH_ERROR_APP_NOT_FOUND;

  if (ret != GAUTH_RETURN_SUCCESS) {
    msg->error_code = ret;
    msg->out_glyph = GLYPH_ERR;
    msg->state = GAUTH_MESSAGE_STATE_MESSAGE_ERROR;
  }

  if (letter_obj)
    json_object_put (letter_obj);

  return ret;
}

static int
gauth_message_process_auth (dbus_msg_t *msg) {
  int ret = gauth_auth (msg, self->symm_key);

  if (ret == GAUTH_RETURN_SUCCESS)
    msg->state = GAUTH_MESSAGE_STATE_FUNCTION_PROCESSED;
  else
    msg->state = GAUTH_MESSAGE_STATE_FUNCTION_PROCESS_ERROR;

  return ret;
}

static int
gauth_message_process_reauth (dbus_msg_t *msg) {
  int ret = gauth_connect ();
  
  if (ret == GAUTH_RETURN_SUCCESS) {
    msg->out_glyph = GLYPH_WHO_OK;
    msg->state = GAUTH_MESSAGE_STATE_GLYPH_PROCESSED;
  } else {
    msg->out_glyph = GLYPH_WHO_NOK;
    msg->state = GAUTH_MESSAGE_STATE_GLYPH_PROCESS_ERROR;
    msg->error_code = ret;
  }
  return ret;
}

static int
gauth_message_process_reload (dbus_msg_t *msg) {
  int ret = gauth_reload ();

  if (ret == GAUTH_RETURN_SUCCESS) {
    msg->out_glyph = GLYPH_RELOAD_OK;
    msg->state = GAUTH_MESSAGE_STATE_GLYPH_PROCESSED;
  } else {
    msg->out_glyph = GLYPH_RELOAD_NOK;
    msg->state = GAUTH_MESSAGE_STATE_GLYPH_PROCESS_ERROR;
    msg->error_code = ret;
  }
  return ret;
}

static int
gauth_message_invocation_return (dbus_msg_t *msg) {
  GLOGI (self->logger, "[MESSAGE OUT: (glyph) %s ::: (response) %s]", msg->out_glyph, msg->out_json);
  g_dbus_method_invocation_return_value (msg->invocation, g_variant_new ("(s)", msg->out_json));
  msg->state = GAUTH_MESSAGE_STATE_RETURNED;
  return GAUTH_RETURN_SUCCESS;
}

static int
gauth_message_invocation_error (dbus_msg_t *msg) {
  if (!msg->out_json) {
    msg->out_json = (char *) calloc (1, GAUTH_MSG_BUF_SIZE);
    snprintf (msg->out_json, GAUTH_MSG_BUF_SIZE, GAUTH_RESP_EFMT, msg->out_glyph, gauth_error_get_string (msg->error_code));
  }
  GLOGE (self->logger, "[MESSAGE ERROR: (code) %d ::: (string) %s]", msg->error_code, gauth_error_get_string (msg->error_code));
  gauth_message_invocation_return (msg);
  return msg->error_code;
}

static int
gauth_message_glyph_return (dbus_msg_t *msg) {
  msg->out_json = (char *) calloc (1, GAUTH_MSG_BUF_SIZE);
  snprintf (msg->out_json, GAUTH_MSG_BUF_SIZE, GAUTH_RESP_FMT, msg->out_glyph);
  gauth_message_invocation_return (msg);
  return GAUTH_RETURN_SUCCESS;
}

static int
gauth_message_glyph_error (dbus_msg_t *msg) {
  msg->out_json = lsf_create_failed_message (GAUTH_DBUS_NAME, gauth_error_get_string (msg->error_code));
  msg->out_json = lsf_encrypt_aes_and_encode_base64 (msg->out_json, self->symm_key);
  msg->out_json = lsf_create_wrapping_message (msg->out_glyph, msg->out_json, 0, NULL);
  GLOGE (self->logger, "[MESSAGE ERROR: (code) %d ::: (string) %s]", msg->error_code, gauth_error_get_string (msg->error_code));
  gauth_message_invocation_return (msg);
  return msg->error_code;
}

static const char *
gauth_state_get_string (int state) {
  const char *gauth_state_string[] = {
    "CREATED", "INITIAILIZED", "LOADED", "CONNECTED", "RUNNING", "DEAD", "DESTROYED"
  };

  return (state < GAUTH_STATE_NUM) ?
         gauth_state_string[state-GAUTH_STATE_CREATED] :
         "UNKNOWN STATE";
}

static const char *
gauth_message_state_get_string (int state) {
  const char *gauth_message_state_string[] = {
    "RECEIVED", "SENDER VERIFIED", "PARSED",
    "GLYPH VERIFIED NORMAL", "GLYPH VERIFIED REAUTH", "GLYPH VERIFIED RELOAD",
    "FUNCTION VERIFIED", "GLYPH PROCESSED", "FUNCTION PROCESSED",
    "MESSAGE ERROR", "GLYPH PROCESS ERROR", "FUNCTION PROCESS ERROR", "RETURNED"
  };

  return (state < GAUTH_MESSAGE_STATE_NUM) ?
         gauth_message_state_string[state-GAUTH_MESSAGE_STATE_RECEIVED] :
         "UNKNOWN STATE";
}

static const char *
gauth_error_get_string (int error_code) {
  return (GAUTH_ERROR_NONE <= error_code && error_code < GAUTH_ERROR_END) ?
         gauth_error_string[error_code-GAUTH_ERROR_NONE] :
         "UNKNOWN ERROR";
}

static int
gauth_app_info_set (const char **app_info) {
  int i;
  app_info_t *app;

  app = (app_info_t*) calloc (1, sizeof (app_info_t));
  if (!app)
    return GAUTH_ERROR_MEMORY_ALLOCATION;
  app->next = NULL;
  for (i = 0; i < GAUTH_APP_POLICY_NUM; i++)
    app->app_policy_field[i] = strdup (app_info[i]);
  if (!self->head) 
    self->head = app;
  else 
    self->tail->next = app;
  self->tail = app;

  return GAUTH_RETURN_SUCCESS;
}

static void
gauth_app_info_print (void) {
  app_info_t *app;

  for (app = self->head; app; app = app->next)
    GLOGI (self->logger, "dbus_name=%s abs_path=%s",
           app->app_policy_field[GAUTH_APP_POLICY_DBUS_NAME],
           app->app_policy_field[GAUTH_APP_POLICY_ABS_PATH]);
}

static app_info_t *
gauth_app_info_find (int arg_flag, const char *arg) {
  app_info_t *app;

  if (arg && RANGE (arg_flag, 0, GAUTH_FIND_APP_FLAG_END)) {
    for (app = self->head; app; app = app->next) {
      switch (arg_flag) {
        case GAUTH_FIND_APP_BY_ABS_PATH:
          if (strcmp (arg, app->app_policy_field[GAUTH_APP_POLICY_ABS_PATH]) == 0)
            return app;
        case GAUTH_FIND_APP_BY_DBUS_NAME:
          if (strcmp (arg, app->app_policy_field[GAUTH_APP_POLICY_DBUS_NAME]) == 0)
            return app;
      }
    }
  }
  return NULL;
}

static void
gauth_app_info_delete (void) {
  app_info_t *del_node;

  while (self->head) {
    del_node = self->head;
    self->head = self->head->next;
    del_node->next = NULL;
    lsf_free (del_node);
  }
  self->tail = self->head;
}

int
main (int argc, char *argv[])
{
  int ret = EXIT_SUCCESS; 

  if (gauth_create () == GAUTH_RETURN_FAILURE)
    exit (EXIT_FAILURE);

  while (self->state != GAUTH_STATE_DEAD) {
    ret = self->gauth_action[self->state]();
    if (ret != GAUTH_RETURN_SUCCESS)
      sleep (5);
  }

  if (self)
    gauth_destroy ();

  exit (ret);
}
