#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define BUFFER 100

int countI = 0;
int count = 0;
int nArguments = 0;

char *argvB[BUFFER];
char *args[BUFFER];
char *auxargs[BUFFER];
char *history[BUFFER];
int h = 0;
int i = 0;
int r = 0;

void removeSpaces(char **str)
{
    while (**str == ' ')
        (*str)++;
}

void ExecSeq()
{
    pid_t cpid[nArguments];

    for (int j = 0; j < nArguments; j++)
    {
        
        cpid[j] = fork();
        separatorArgv();

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

    printf("\nTotal Commands executed: %d  >>\t", countI);

    for (int m = 0; m < countI; m++)
    {
        printf("%s; ", args[m]);
    }
    printf("\n\n");
    printf("Run in sequential with sucess!\n");
    countI = 0;
    count = 0;
}

void ExecParal() {

    pid_t cpid[nArguments];
    int cpyArgs = nArguments;

    for (int j = 0; j < nArguments; j++)
    {       
        separatorArgv();

        cpid[j] = fork();

        if (cpid[j] < 0)
        {
            printf("Fork failed!\n");
        }

        else if (cpid[j] == 0)
        {
            execvp(auxargs[0], auxargs);
        }

        for (int k = 0; k < count; k++)
        {
            auxargs[k] = NULL;
        }
        countI++;
        count = 0;
    }

   
    while (cpyArgs > 0)
    {
        
        wait(NULL);
        cpyArgs--;
    }

    printf("\nAll commands executed: \n");

    for (int j = 0; j < nArguments; j++)
    {
        printf("%s\n", args[j]);
    }
    printf("\nExec in Parallel with sucess! Exiting Shell...\n");
    exit(0);
}

void separatorArgv()
{
    char *ptr;
    ptr = strtok(args[countI], " ");

    while (ptr != NULL)
    {
        auxargs[count] = ptr;
        history[h] = ptr;
        count++;
        h++;
        ptr = strtok(NULL, " ");
    }
    auxargs[count] = NULL;
}

void separatorInput()
{
    char *separatorStr;

    separatorStr = strtok(argvB[r], ";");

    while (separatorStr != NULL)
    {
        //args[nArguments] = malloc(sizeof *args[nArguments] * BUFFER);
        args[nArguments] = separatorStr;
        removeSpaces(&args[nArguments]);
        nArguments++;
        separatorStr = strtok(NULL, ";");
    }
    args[nArguments] = NULL;
}

int main(int argc, char **argv) {
    
    FILE *file;

    if(argc < 2) {

        printf("No input arguments!\n");
    }
    if(argc >= 2) {

        file = fopen(argv[1], "r");

        while(!feof(file) && !ferror(file)) {

            argvB[i] = malloc(sizeof *argvB[i] * BUFFER);
            fgets(argvB[i], BUFFER, file);
            argvB[i][strlen(argvB[i]) - 1] = '\0';
            separatorInput();      
            i++;
            r++;
        }

        printf("\nExec in Seq >\n");
        ExecSeq();

        printf("\nExec in Parallel >\n");
        ExecParal();

       
        fclose(file);
    }

    return 0;
}