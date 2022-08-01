#ifndef __LSF_LOGGER_H__
#define __LSF_LOGGER_H__

#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "lsf-confer.h"

#define LOGGER_MAX              10
#define LOGGER_FMT_STRING_BUF   4096
#define LOGGER_FMT_WORD_BUF     10
#define LSF_LOGGER_DEFAULT_NAME "lsf"

enum {
  LOGGER_NAME,
  LOGGER_PATH,
  LOGGER_FMT,
  LOGGER_LEVEL,
  LOGGER_FILE_EXTENSION,
  LOGGER_CONFIG_NUMS
};

enum {
  LOGGER_LEVEL_DEBUG,
  LOGGER_LEVEL_INFO,
  LOGGER_LEVEL_WARNING,
  LOGGER_LEVEL_ERROR,
  LOGGER_LEVEL_CRITICAL,
  LOGGER_LEVEL_NUMS
};

enum {
  LOGGER_PRINT_SUCCESS,
  LOGGER_PRINT_FAILURE_LOGGER_IS_NULL,
  LOGGER_PRINT_FAILURE_CANNOT_OPEN_DIR,
  LOGGER_PRINT_FAILURE_CANNOT_OPEN_FILE,
  LOGGER_PRINT_RESULT_NUMS
};

typedef struct {
  gchar   *logger_config[LOGGER_CONFIG_NUMS];
  gint     log_level_index;
  gint     max_bytes;
  gint     backup_count;
  FILE    *output_fp;
} Logger;

Logger *logger[LOGGER_MAX];
int     logger_cnt;

#ifndef LOGGER_USE
#define GLOGC(logger, format, ...) lsf_logger_print(logger, "CRITICAL", __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define GLOGE(logger, format, ...) lsf_logger_print(logger, "ERROR", __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define GLOGW(logger, format, ...) lsf_logger_print(logger, "WARNING", __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define GLOGI(logger, format, ...) lsf_logger_print(logger, "INFO", __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define GLOGD(logger, format, ...) lsf_logger_print(logger, "DEBUG", __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#else
#define GLOGC(logger, format, ...)
#define GLOGE(logger, format, ...)
#define GLOGW(logger, format, ...)
#define GLOGI(logger, format, ...)
#define GLOGD(logger, format, ...)
#endif //LOGGER_USE

extern Logger  *lsf_get_logger                      (char *logger_name);
extern void     lsf_logger_free                     (Logger *logger);
extern int      lsf_logger_print                    (Logger *logger, char *log_level, const char *file_name, const int line_num, const char *func_name, char *format, ...);

#endif
