#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <readline/readline.h>
#include <readline/history.h>

#define MAX_STRING 1024

int execute_command(char** args);
char* type_prompt (void);
char** read_command (int* nargs);
int execute_chown(char* path, char* group);
int execute_date();
