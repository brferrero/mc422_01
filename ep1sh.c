#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wordexp.h>  /*Para fazer o parse dos argumentos*/

int type_prompt (void)
{   
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s]$ ", cwd);
    return 0;
}

int read_command (char **args, int *nargs) {
    char command[100];
    char *token;
    int i = 1;

    fgets(command, 100, stdin);
    token = strtok(command, " ");
    
    args = malloc(sizeof(char*) * 10);  
    args[0] = malloc(sizeof(char)*20);
    sscanf(command, "%s%*[^\n]",args[0]);  
    while( token != NULL ) {
        args[i] = malloc(sizeof(char)*strlen(token)+1);
        strcpy(args[i],token);
        token = strtok(NULL, " ");
        i++;
    }

    *nargs = i;
    return 0;
}

void forkar()
{
    pid_t childpid;
    if ( (childpid = fork()) != 0) {
        /* Processo PAI */
        childpid = waitpid (-1, NULL, 0); /* Espera processo filho terminar */
        /* printf ("childpid : %d \n",childpid);
        // sleep(3); */
    }
    else {
        /* Processo FILHO*/  

        /*printf ("exec comando: %s \n",argv[1]);*/
        sleep(3);
    }
}

int main (int argc, char **argv) {
    char **args;
    int nargs;
    char command[100];
    char *token;
    int i = 0;
  
    while (1) {
        type_prompt();
        fgets(command, 100, stdin);
        token = strtok(command, " ");
        
        args = malloc(sizeof(char*) * 10);  
        while( token != NULL ) {
            args[i] = malloc(sizeof(char)*strlen(token)+1);
            strcpy(args[i],token);
            token = strtok(NULL, " ");
            i++;
        }

        nargs = i;
      
        for (i = 0; i < nargs; i++)
            printf("\nArgumento %d: %s", i, args[i]);
        
    }
    exit(0);
}
