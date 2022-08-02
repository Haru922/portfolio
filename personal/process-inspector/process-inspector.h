#ifndef __PROCESS_INSPECTOR_H__
#define __PROCESS_INSPECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CMD_MAX 128*1024

char * get_pid_command (unsigned int pid);
char * get_pid_abspath (unsigned int pid);
char * get_pid_path (unsigned int pid);

#endif
