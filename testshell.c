#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_ARGS 80
#define BUFFER 1024

char argv[MAX_ARGS][BUFFER] = {"ls -l", "ps", "pwd", "echo 123", "echo hello!", "ls"};
char cpy[MAX_ARGS][BUFFER];
// an array for command and arguments
char *auxargs[MAX_ARGS / 2 + 1];
int argc = 0;
int statusexit = 0;
int count = 0;
int i = 6;
int countI = 0;

void removeSpaces(char **str)
{
    while (**str == ' ')
        (*str)++;
}

void separatorArgv(char argv[MAX_ARGS][BUFFER])
{

    char *ptr;

    ptr = strtok(argv[countI], " ");

    while (ptr != NULL)
    {

        auxargs[count] = ptr;
        count++;
        ptr = strtok(NULL, " ");
    }
    auxargs[count] = NULL;
}

void runShellSeq() {

    for (int j = 0; j < i; j++)
    {
        pid_t cpid[i];

        cpid[j] = fork();

        separatorArgv(argv);

        if (cpid[j] < 0)
        {
            printf("Fork failed!\n");
        }
        else if (cpid[j] == 0)
        {
            execvp(auxargs[0], auxargs);
        }
        else
        {
            printf("Command: %s\n", auxargs[0]);
            wait(NULL);
        }
        // Clear array
        for (int k = 0; k < i; k++)
        {
            auxargs[k] = NULL;
        }
        countI++;
        count = 0;
    }

    printf("Total Commands executes: %d  >>\t", countI);

    for (int m = 0; m < countI; m++)
    {

        printf("%s; ", cpy[m]);
    }

    printf("\n\n");
    printf("Exit sucess!\n");
    exit(0);
}

void runShellPar() {




}
int main () {


    for (int j = 0; j < i; j++)
    {
        strcpy(cpy[j], argv[j]);
    }
    while(1) 
    
        runShellSeq();

    return 0;
}