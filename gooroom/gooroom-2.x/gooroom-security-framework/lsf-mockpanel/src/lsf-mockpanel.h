#ifndef __LSF_MOCKPANEL_H__
#define __LSF_MOCKPANEL_H__

#define LSF_CC_PANEL_DIR              "/var/tmp/lsf/lsf-cc-panel"

#define LSF_MOCKPANEL_NAME            "lsf-mockpanel"
#define LSF_MOCKPANEL_DBUS_NAME       "kr.gooroom.lsf-mockpanel"

#define LSF_MOCKPANEL_WIDTH           750
#define LSF_MOCKPANEL_HEIGHT          620

#define MAX_APP_NUM                   100

#define SET_SETTINGS_FMT              "{ \"to\": \"%s\", \"from\": \"%s\", \"access_token\": \"%s\", \"function\": \"%s\", \"app_conf\": %s }"
#define GET_SETTINGS_FMT              "{ \"to\": \"%s\", \"from\": \"%s\", \"access_token\": \"%s\", \"function\": \"%s\" }"

#endif
