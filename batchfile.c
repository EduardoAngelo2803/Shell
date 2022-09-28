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
char *args[BUFFER];
char *auxargs[BUFFER];
size_t i = 0;

void ExecSeq()
{
    pid_t cpid[i];

    for (int j = 0; j < i; j++)
    {
        
        cpid[j] = fork();

        separatorArgv(&args);

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
            printf("Command: %s\n", args[j]);
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

    pid_t cpid[i];
   
    for (int j = 0; j < i; j++)
    {
        cpid[j] = fork();

        separatorArgv(&args);

        if (cpid[j] < 0)
        {
            printf("Fork failed!\n");
        }
        else if (cpid[j] == 0)
        {
            execvp(auxargs[0], auxargs);
        }

        for (int k = 0; k < i; k++)
        {
            auxargs[k] = NULL;
        }
        countI++;
        count = 0;
        
    }

    while (i > 0)
    {
        wait(NULL);
        i--;
    }

}

void separatorArgv(char *argvv[BUFFER])
{

    char *ptr;

    ptr = strtok(argvv[countI], " ");

    while (ptr != NULL)
    {
        auxargs[count] = ptr;
        count++;
        ptr = strtok(NULL, " ");
    }
    auxargs[count] = NULL;
}

int main(int argc, char **argv) {

    FILE *file;
    
    if(argc < 2) {

        printf("No input arguments!\n");
    }
    if(argc >= 2) {

        file = fopen(argv[1], "r");


        while(!feof(file) && !ferror(file)) {

            args[i] = malloc(sizeof *args[i] * BUFFER);
            fgets(args[i], BUFFER, file);
            i++;
        }

        for (int m = 0; m < i; m++) {

            args[m][strlen(args[m]) - 1] = '\0';
        }

        printf("\nExec in Seq >\n");
        ExecSeq();

        printf("\nExec in Parallel >\n");
        ExecParal();
        
        fclose(file);
    }

    return 0;
}