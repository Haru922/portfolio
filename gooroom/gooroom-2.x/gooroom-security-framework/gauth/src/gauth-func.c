#include "gauth-func.h"
#include "lsf/lsf-auth.h"
#include "lsf-util-common.h"
#include "lsf-crypto-common.h"

int
gauth_auth (dbus_msg_t *msg,
            char       *auth_symm_key)
{
  json_object *req_obj = NULL;
  json_object *params_obj = NULL;
  json_object *iter_obj = NULL;
  json_object *resp_obj = NULL;
  json_object *ret_obj = NULL;
  unsigned char *private_key = NULL;
  char *access_token = NULL;
  unsigned char *decoded_priv_key = NULL; 
  char *response = NULL;
  char *encrypt_msg = NULL;
  unsigned char *encrypted_path = NULL;
  char *decrypted_path = NULL;
  int decoded_len = 0;
  int key_len = 0;
  int ret = GAUTH_RETURN_SUCCESS;
  char **app_policy_field = msg->target_app->app_policy_field;

  req_obj = json_tokener_parse (msg->letter);
  if (!req_obj) {
    msg->error_code = GAUTH_ERROR_JSON_PARSING;
    goto GAUTH_AUTH_END;
  }

  decoded_priv_key = lsf_decode_Base64 (app_policy_field[GAUTH_APP_POLICY_PRIVATE_KEY], &decoded_len);

  if (!json_object_object_get_ex (req_obj, "params", &params_obj) 
      || !json_object_object_get_ex (params_obj, "passphrase", &iter_obj)) {
    msg->error_code = GAUTH_ERROR_JSON_OBJECT_GET;
    goto GAUTH_AUTH_END;
  }
  key_len = lsf_decrypt_AES (decoded_priv_key,
                             &private_key,
                             decoded_len,
                             json_object_get_string (iter_obj));

  if (!json_object_object_get_ex (params_obj, "abs_path", &iter_obj)) {
    msg->error_code = GAUTH_ERROR_JSON_OBJECT_GET;
    goto GAUTH_AUTH_END;
  }
  encrypted_path = lsf_decode_Base64 (json_object_get_string (iter_obj), &decoded_len);

  lsf_decrypt_RSA (encrypted_path, &decrypted_path, decoded_len, private_key, key_len);

  if (!json_object_object_get_ex (params_obj, "ghub_abspath", &iter_obj)) {
    msg->error_code = GAUTH_ERROR_JSON_OBJECT_GET;
    goto GAUTH_AUTH_END;
  }
  if (decrypted_path &&
     (strcmp (decrypted_path, json_object_get_string (iter_obj)) == 0)) {
    access_token = lsf_create_access_token ("gauth", auth_symm_key, atoi (app_policy_field[GAUTH_APP_POLICY_EXP]));
    ret = lsf_create_dbus_conf_file (app_policy_field[GAUTH_APP_POLICY_DBUS_NAME]);

    if (ret == GAUTH_RETURN_SUCCESS) {
      ret_obj = json_object_new_object ();
      json_object_object_add (ret_obj, "status", json_object_new_string ("success"));
      json_object_object_add (ret_obj, "access_token", json_object_new_string (access_token));
      json_object_object_add (ret_obj, "bus_name", json_object_new_string (app_policy_field[GAUTH_APP_POLICY_DBUS_NAME]));
      json_object_object_add (ret_obj, "display_name", json_object_new_string (app_policy_field[GAUTH_APP_POLICY_DISPLAY_NAME]));

      if (!json_object_object_get_ex (req_obj, "from", &iter_obj)) {
        msg->error_code = GAUTH_ERROR_JSON_OBJECT_GET;
        goto GAUTH_AUTH_END;
      }
      resp_obj = lsf_create_json_response (GAUTH_DBUS_NAME,
                                           json_object_get_string (iter_obj),
                                           "auth",
                                           ret_obj);
      response = strdup (json_object_to_json_string (resp_obj));
    } else
      msg->error_code = GAUTH_ERROR_DBUS_CONF_CREATE;
  } else
    msg->error_code = GAUTH_ERROR_ABS_PATH_INVALID;

GAUTH_AUTH_END:
  if (msg->error_code != GAUTH_ERROR_NONE) {
    response = lsf_create_failed_message (GAUTH_DBUS_NAME, gauth_error_string[msg->error_code-GAUTH_ERROR_NONE]);
    ret = msg->error_code;
  }

  encrypt_msg = lsf_encrypt_aes_and_encode_base64 (response, auth_symm_key);
  msg->out_json = lsf_create_wrapping_message (msg->out_glyph, encrypt_msg, 0, NULL);

  lsf_free (response);
  lsf_free (encrypt_msg);

  json_object_put (resp_obj);
  json_object_put (ret_obj);
  json_object_put (req_obj);

  return ret;
}
