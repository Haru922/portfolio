#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CMD_MAX 131070

int main (int argc, char **argv) {
  int i;
  int pid;
  char *p = NULL;
  FILE *fp = NULL;
  char cmdline[PATH_MAX] = { 0, };
  char abspath[PATH_MAX] = { 0, };
  char buf[CMD_MAX] = { 0, };

  fprintf (stdout, "** cmd_parser **\n");
  for (;;) {
    pid = -1;
    fprintf (stdout, "PID(0-32768): ");
    fscanf (stdin, "%d", &pid);
    if (pid < 0)
      break;
    snprintf (cmdline, PATH_MAX, "/proc/%d/cmdline", pid);
    fp = fopen (cmdline, "r");
    if (fp) {
      fgets (buf, CMD_MAX, fp);
      fclose (fp);
      if (buf) {
        fprintf (stdout, "cmd: %s", buf);
        p = strchr (buf, '\0');
        while (p && p[1]) {
          fprintf (stdout, " %s", p+1);
          p = strchr (p+1, '\0');
        }
        putchar ('\n');
      } 
    } else 
      fprintf (stderr, "Cannot get command\n");
    snprintf (abspath, PATH_MAX, "/proc/%d/exe", pid);
    fprintf (stdout, "abspath: %s\n", realpath (abspath, NULL));
    for (i=0; i<CMD_MAX; i++)
      buf[i] = '\0';
    fp = NULL;
    p = NULL;
  }
  fprintf (stdout, "EXIT\n");

  exit (EXIT_SUCCESS);
}
