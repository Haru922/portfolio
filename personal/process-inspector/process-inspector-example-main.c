#include "process-inspector.h"

int main (int argc, char **argv) {
  unsigned int pid = 0;
  char *path = NULL;
  
  fprintf (stdout, "** cmd_parser **\n");
  for (;;) {
    pid = 0;
    fprintf (stdout, "PID(0-32768): ");
    fscanf (stdin, "%u", &pid);
    if (pid == 0)
      break;

    path = get_pid_path (pid);
    if (path) {
      fprintf (stdout, "path: %s\n", path);
      free (path);
      path = NULL;
    }
  }
  fprintf (stdout, "EXIT\n");

  exit (EXIT_SUCCESS);
}
