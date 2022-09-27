#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SIZE_CMD 256
#define MAX_SIZE_ARG 16

char cmd[MAX_SIZE_CMD];   // string holder for the command
char *args[10]; // an array for command and arguments
              // global variable for the child process ID
int i = 10;                   // global for loop counter

/*void get_cmd();           // get command string from the user
void convert_cmd();       // convert the command string to the required format by execvp()
void c_shell();       */    // to start the shell
void log_handle(int sig);// signal handler to add log statements

int main () {

    /*char *arg[MAX_SIZE_ARG] = {{"ls", "-l", NULL}, {"ps", NULL}, {"pwd", NULL}, {"echo", "Hello Shell!", NULL}, 
    {"echo", "Run BathcFile!", NULL}};
    char *aux[MAX_SIZE_ARG];

    for (int j = 0; j < 5; j++) {

        pid_t cpid[10];
        cpid[j] = fork();
        aux[0] = arg[j];
        aux[1] = NULL;

        if (cpid[j] < 0)
        {

            printf("Fork failed!\n");
        }
        else if (cpid[j] == 0)
        {

            execvp(aux[0], aux);

        }else {

            printf("Command executing now..: %s\n", aux[0]);
            aux[j] = NULL;
            wait(NULL);
        }
            
        

        
    }*/

    char *argv[MAX_SIZE_ARG] = {{"ps", NULL}, {"ls -l", NULL}};
    char *auxargs[MAX_SIZE_ARG];
    char *str;
    int i = 0;
    auxargs[0] = argv[1];
    printf("%s", auxargs[0]);
    str = strtok(auxargs[0], " ");

   /* while (str != NULL)
    {

        auxargs[i] = str;
        i++;
        str = strtok(NULL, " ");
    }
    printf("%s", auxargs[0]);*/
    return 0;
}