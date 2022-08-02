#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  pid_t pid;
  int i,j,status;
  char *command[argc];

  for (i=2,j=0; i<argc; i++)
    command[j++] = argv[i];
  command[j] = NULL;

  pid = fork ();
  if (pid == -1) {
    fprintf (stderr, "cannot fork, error occured\n");
    exit (EXIT_FAILURE);
  } else if (pid == 0) {
    fprintf (stdout, "child process pid: %u\n", getpid());
    fprintf (stdout, "parent process pid: %u\n", getppid());
    execv (argv[1], command);
    exit (EXIT_SUCCESS);
  } else {
    if (waitpid (pid, &status, 0) > 0) {
      if (WIFEXITED (status) && !WEXITSTATUS (status)) 
        fprintf (stdout, "\n** SUCCESS **\n");
      else {
        fprintf (stderr, "\n** FAILURE **\n");
        exit (EXIT_FAILURE);
      }
      exit (EXIT_SUCCESS);
    } else
      exit (EXIT_FAILURE);
  }
}
