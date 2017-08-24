#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int type_prompt (void)
{   
    printf ("dir atual $ : ");
    return 1;
}

int read_command (char * command) {
    fgets (command, 50, stdin);
    return 1;
}

int main (int argc, char **argv) {
    
    pid_t childpid;
    char command[50];
    strcpy (command, "pwd");
    system (command);
    
    while (1) {
        type_prompt ();

        if ( (childpid = fork()) != 0) {
            /* espera filho */
            childpid = waitpid (-1, NULL, 0);
            printf ("childpid : %d \n",childpid);
            sleep(3);
        }
        else {
            /*filho*/
            read_command (command);
            printf ("exec comando: %s \n",command);
            sleep(3);
        }
    }
    exit(0);
}
