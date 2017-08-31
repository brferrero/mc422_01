#include "ep1sh.h"

int main (int argc, char **argv)
{
    char **args;
    int nargs;
    /*int i = 0;*/
  
    while (1) {
        
        args = read_command(&nargs);
        if (execute_command(args) == -1) 
            break;

        /*for (i = 0; i < nargs; i++)
            printf("\nArgumento %d: %s", i, args[i]);
        */
    }
    exit(0);
}

char* type_prompt (void)
{   
    char prompt[MAX_STRING], cwd[MAX_STRING];
    char *command;

    getcwd(cwd, sizeof(cwd));
    sprintf(prompt,"[%s]$ ", cwd);

    command = readline(prompt);
    if (command)
         add_history(command);

    return command;
}

char** read_command (int *nargs)
{
    char **args, *command, *token;
    int i = 0;

    command = type_prompt();    

    /* Parser de argumentos da linha de comando */
    args = malloc(sizeof(char*) * 10);  
    token = strtok(command, " ");
    while( token != NULL ) {
        args[i] = malloc(sizeof(char)*MAX_STRING);
        strcpy(args[i],token);
        token = strtok(NULL, " ");
        i++;
    }

    /* Numero de argumentos digitados */
    *nargs = i;
    return args;
}

int execute_command(char** args)
{
    char *envp[] = { NULL };
    pid_t childpid;

    /* sai do shell */
    if (strcmp("exit", args[0]) == 0) {
        printf ("Saindo do o shell...\n");
        return -1;
    }
    else {
        if (strcmp("chown", args[0]) == 0)
            execute_chown(args[2], args[1]+1);
        else 
            if (strcmp("date", args[0]) == 0)
                execute_date();
            else {
                if ((childpid = fork()) != 0) {
                    /* Processo PAI */
                    childpid = waitpid (-1, NULL, 0); /* Espera processo filho terminar */
                }
                else {
                    /* Processo FILHO*/  
                    if (execve(args[0], args, envp) == -1 )
                        printf ("%s : nao encontrado\n", args[0]);
                }
            }
        return 0;
    }
}

int execute_chown(char* path, char* group)
{
    struct group *g;
    struct stat file;
    gid_t gid;
    uid_t uid;

    g = getgrnam(group);
    stat(path, &file);

    gid = g->gr_gid;
    uid = file.st_uid;

    chown(path, uid, gid);
    return 0;
}

int execute_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
  
    time (&rawtime);
    timeinfo = localtime(&rawtime);
  
    strftime (buffer,80,"%a %b %d %H:%M:%S %Z %Y",timeinfo);
    printf ("%s\n", buffer);

    return 0;
}
