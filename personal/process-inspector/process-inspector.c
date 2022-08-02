#include "process-inspector.h"
#include <sys/stat.h>
#include <sys/types.h>

char *
get_pid_command (unsigned int pid)
{
  int i = 0;
  FILE *fp = NULL;
  char *p = NULL;
  char *cmd = NULL;
  char buf[CMD_MAX+1] = { 0, };

  snprintf (buf, PATH_MAX+1, "/proc/%u/cmdline", pid);
  fp = fopen (buf, "r");
  if (fp) {
    for (i=0; i<CMD_MAX+1; i++)
      buf[i] = '\0';
    fgets (buf, CMD_MAX+1, fp);
    fclose (fp);
    if (buf) {
      cmd = (char *) calloc (1, sizeof (char) * (CMD_MAX+1));
      snprintf (cmd, CMD_MAX+1, "%s", buf);
      p = strchr (buf, '\0');
      while (p && p[1]) {
        strncat (cmd, " ", CMD_MAX);
        strncat (cmd, p+1, CMD_MAX);
        p = strchr (p+1, '\0');
      }
    }
  }

  return cmd;
}

char *
get_pid_abspath (unsigned int pid)
{
  char buf[PATH_MAX+1] = { 0, };

  snprintf (buf, PATH_MAX+1, "/proc/%u/exe", pid);

  return realpath (buf, NULL);
}

char *
get_pid_path (unsigned int pid)
{
  char *p = NULL;
  char *path = NULL;
  char *abspath = get_pid_abspath (pid);
  char *cmd = get_pid_command (pid);

  if (abspath && cmd) {
    if (strcmp (abspath, cmd)) {
      path = (char *) calloc (1, sizeof (char) * (CMD_MAX+1));
      snprintf (path, CMD_MAX+1, "%s", abspath);
      p = strchr (cmd, ' ');
      if (p && p[1]) {
        strncat (path, " ", CMD_MAX);
        strncat (path, p+1, CMD_MAX);
      }
    } else {
      path = abspath;
      free (cmd);
    }
  } else 
    path = (abspath) ? abspath : cmd;

  return path;
}
