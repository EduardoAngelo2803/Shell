#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAX_ARGS 80
#define BUFFER 1024

int flags;
pid_t pidPipe[2];
char *arguments[MAX_ARGS / 2 + 1];
char *auxargs[MAX_ARGS / 2 + 1];
char *argBatch[MAX_ARGS / 2 + 1];
char input_user[BUFFER];
char guideInput[BUFFER];
int countHistory = 0;
int flagChoose;
int flagH = 0;
char input_initial[30];
int countGlobal = 0;
int statusexit = 0;
int countI = 0;
int count = 0;
int i = 0;
int r = 0;
int contBatch = 0;
int contador = 0;

void removeSpaces(char **str)
{

    while (**str == ' ')
        (*str)++;
}

void get_input()
{
    // get command from user
    if (flagChoose == 1)
    {

        printf("leam seq> ");
    }
    else if (flagChoose == 2)
    {

        printf("leam par> ");
    }

    memset(input_user, 0, sizeof(char) * BUFFER);

    fgets(input_user, BUFFER, stdin);

    if (input_user == NULL)
    {
        flagChoose = 3;
        exit(0);
    }
    input_user[strlen(input_user) - 1] = '\0';
    // Coloca os comandos no array auxiliar, caso não seja pressionado !!. Pois se for
    // Ele passa direto, logo, fica salvo o ultimo input antes do '!!'.
    if (strcmp(input_user, "!!") != 0)
    {

        strcpy(guideInput, input_user);
    }
    if (strcmp(input_user, "!!") == 0)
    {

        if (countGlobal == 0)
        {

            printf("No commands\n");
        }
        flagH = 1;
    }

    if (strcmp(input_user, "exit") == 0)
    {

        printf("Ending our Shell application, GoodBye!\n");
        statusexit = 1;
        exit(0);
    }
}

void separatorarguments(char *argumentss[BUFFER]) {

    char *ptr;

    ptr = strtok(arguments[countI], " ");

    while (ptr != NULL)
    {

        auxargs[count] = ptr;
        count++;
        ptr = strtok(NULL, " ");
    }
    auxargs[count] = NULL;
}

void VerPipeBatch (char *argBatchh[41]) {

    char *delimit2 = ">;<|";
    char *verDelim2 = strdup(argBatchh[contBatch]);
    char *separatorBatch;

    separatorBatch = strtok(argBatchh[contBatch], delimit2);

    if (verDelim2[separatorBatch - argBatchh[contBatch] + strlen(argBatchh[contBatch])] == ';')
      {
          flags = 1;
      }
      else if (verDelim2[separatorBatch - argBatchh[contBatch] + strlen(argBatchh[contBatch])] == '|')
      {
          flags = 2;
      }
      else if (verDelim2[separatorBatch - argBatchh[contBatch] + strlen(argBatchh[contBatch])] == '>')
      {
          flags = 3;
      }
      else if (verDelim2[separatorBatch - argBatchh[contBatch] + strlen(argBatchh[contBatch])] == '<')
      {
          flags = 4;
      }

    while (separatorBatch != NULL)
    {
        arguments[i] = separatorBatch;
        removeSpaces(&arguments[i]);
        i++;
        separatorBatch = strtok(NULL, delimit2);
    }
    arguments[i] = NULL;
    free(verDelim2);
}

void separgsBatch(char *argBatchh[41])
{
    char *delimit2 = "<;|>";
    //  char *verDelim2 = strdup(argBatch[contBatch]);
    char *separatorBatch;
    separatorBatch = strtok(argBatchh[contBatch], delimit2);
    
    while (separatorBatch != NULL)
    {
        arguments[i] = separatorBatch;
        removeSpaces(&arguments[i]);
        i++;
        separatorBatch = strtok(NULL, delimit2);
    }
    arguments[i] = NULL;
    // free(verDelim2);
}

void separatorInput(char input_user[BUFFER])
{
    
    char *delimit = "<;|>";
    char *verDelim = strdup(input_user);
    char *separatorStr;
    i = 0;

    separatorStr = strtok(input_user, delimit);
    flags = 0;
    if (verDelim[separatorStr - input_user + strlen(input_user)] == ';')
    {

        flags = 1;
    }
    else if (verDelim[separatorStr - input_user + strlen(input_user)] == '|')
    {

        flags = 2;
    }
    else if (verDelim[separatorStr - input_user + strlen(input_user)] == '>')
    {

        flags = 3;
        if (verDelim[separatorStr - input_user + strlen(input_user) + 1] == '>')
        {

            flags = 5;
        }
    }
    else if (verDelim[separatorStr - input_user + strlen(input_user)] == '<')
    {

        flags = 4;
        
     }

        while (separatorStr != NULL)
        {

            arguments[i] = separatorStr;
            removeSpaces(&arguments[i]);
            i++;
            separatorStr = strtok(NULL, delimit);
        }

    free(verDelim);
}

void execSeqBatch()
{

    for (int j = 0; j < i; j++)
    {

        pid_t cpid[i];
        cpid[j] = fork();

        separatorarguments(arguments);

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

    printf("Executing in Sequence done with sucess!\n");
    printf("Executed command History: \n");

    for (int k = 0; k < i; k++)
    {

        printf("%s; ", arguments[k]);
    }

    printf("\n\n");
    countI = 0;
    count = 0;
}

void execParalBatch()
{

    pid_t cpid[i];
    int cpyArgs = i;

    memset(auxargs, 0, 41);

    for (int j = 0; j < i; j++)
    {
        separatorarguments(arguments);

        cpid[j] = fork();

        if (cpid[j] < 0)
        {
            printf("Fork failed!\n");
        }

        else if (cpid[j] == 0)
        {
            execvp(auxargs[0], auxargs);
        }

        memset(auxargs, 0, 41);

        countI++;
        count = 0;
    }

    while (cpyArgs > 0)
    {

        wait(NULL);
        cpyArgs--;
    }

    printf("\nAll commands executed: \n");

    for (int j = 0; j < i; j++)
    {
        printf("%s\n", arguments[j]);
    }
    printf("\nExec in Parallel with sucess! Exiting Shell...\n");
    exit(0);
}

void execPid(char *arguments[41])
{

    countGlobal++;

    if (flagH == 1)
    {

        separatorInput(guideInput);
        flagH = 0;
    }

    for (int j = 0; j < i; j++)
    {
        pid_t cpid[i];
        cpid[j] = fork();

        separatorarguments(arguments);

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
            printf("Command: %s\n", arguments[j]);
            wait(NULL);
        }
        // Clear array
        memset(auxargs, 0, sizeof(char) * 41);

        countI++;
        count = 0;
    }
}

void PipeSequential()
{

    int fd[2];
    pipe(fd);
    pidPipe[0] = fork();
    separatorarguments(arguments);
    if (flagH == 1)
    {
        separatorInput(guideInput);
        flagH = 0;
    }
    if (pidPipe < 0)
    {

        printf("Error!\n");
    }
    else if (pidPipe[0] == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execvp(auxargs[0], auxargs);
    }

    memset(auxargs, 0, sizeof(char) * 41);

    countI++;
    count = 0;
    separatorarguments(arguments);

    pidPipe[1] = fork();

    if (pidPipe[1] < 0)
    {
        printf("Error!\n");
    }
    else if (pidPipe[1] == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(auxargs[0], auxargs);
    }
    // close(STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

void PipeParallel()
{

    int fd[2];
    pipe(fd);
    pidPipe[0] = fork();
    separatorarguments(arguments);
    if (flagH == 1)
    {
        separatorInput(guideInput);
        flagH = 0;
    }
    if (pidPipe < 0)
    {

        printf("Error!\n");
    }
    else if (pidPipe[0] == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execvp(auxargs[0], auxargs);
    }

    memset(auxargs, 0, sizeof(char) * 41);

    countI++;
    count = 0;
    separatorarguments(arguments);

    pidPipe[1] = fork();

    if (pidPipe[1] < 0)
    {
        printf("Error!\n");
    }
    else if (pidPipe[1] == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(auxargs[0], auxargs);
    }
    // close(STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

void readFileSeq()
{

    separatorarguments(arguments);

    int fd = open(arguments[1], O_CREAT | O_WRONLY | O_TRUNC);

    pid_t pid1;

    pid1 = fork();

    if (fd == -1)
    {
        perror("Error open file!\n");
        exit(1);
    }
    if (pid1 < 0)
    {

        printf("Error!");
    }
    else if (pid1 == 0)
    {

        fclose(stdout);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(auxargs[0], auxargs);
    }
    else
    {

        wait(NULL);
    }
    printf("Created a file: %s with outputs of command: %s!\n", arguments[1], auxargs[0]);

    for (int k = 0; k < i; k++)
    {

        auxargs[k] = NULL;
    }

    countI++;
    count = 0;
}

void recepOut()
{

    FILE *file;
    file = fopen(arguments[1], "r");
    r = 0;

    memset(arguments, 0, sizeof(char) * (MAX_ARGS / 2 + 1));

    while (!feof(file) && !ferror(file))
    {

        argBatch[r] = malloc(sizeof *argBatch[r] * 41);
        fgets(argBatch[r], 41, file);
        argBatch[r][strlen(argBatch[r]) - 1] = '\0';
        r++;
    }
    i = 0;
    for (int s = 0; s < r - 1; s++)
    {
        separgsBatch(argBatch);
        contBatch++;
    }
    
    execPid(arguments);
    fclose(file);
}

void append() {

    separatorarguments(arguments);
    FILE *file;

    int fd = open(arguments[1], O_WRONLY | O_APPEND);

    pid_t pid1;

    pid1 = fork();

    if (fd == -1)
    {
        perror("Error open file!\n");
        exit(1);
    }
    if (pid1 < 0)
    {

        printf("Error!");
    }
    else if (pid1 == 0)
    {

        fclose(stdout);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(auxargs[0], auxargs);
    }
    else
    {

        wait(NULL);
    }
    printf("Output of command %s appended in %s!\n", auxargs[0], arguments[1]);

    for (int k = 0; k < i; k++)
    {

        auxargs[k] = NULL;
    }

    countI++;
    count = 0;
}

void runInSequential()
{

    if (statusexit != 1)
    {

        while (1)
        {

            memset(arguments, 0, sizeof(char) * (MAX_ARGS / 2 + 1));
            countI = 0;
            get_input();
            if (strcmp("style parallel", input_user) == 0)
            {

                flagChoose = 2;
                break;
            }

            if (strcmp("exit", input_user) == 0 || input_user == NULL)
            {

                exit(0);
            }

            if (flagH == 0)
            {

                separatorInput(input_user);
            }
            if (flags == 1 || flags == 0)
            {

                execPid(arguments);
            }
            else if (flags == 2)
            {

                PipeSequential();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
            else if (flags == 3)
            {

                readFileSeq();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
            else if (flags == 4)
            {

                recepOut();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
            else if (flags == 5) {
               
                append();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
        }
    }
}

void execParalel(char *arguments[BUFFER])
{

    pid_t cpid[i];
    int j = 0;

    for (j = 0; j < i; j++)
    {
        cpid[j] = fork();

        separatorarguments(arguments);

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

    while (countI > 0)
    {
        wait(NULL);
        countI--;
    }
}

void runInParallel()
{

    if (statusexit != 1)
    {

        memset(arguments, 0, sizeof(char) * (MAX_ARGS / 2 + 1));

        countI = 0;

        while (1)
        {
            get_input();
            if (strcmp("style sequential", input_user) == 0)
            {

                flagChoose = 1;
                break;
            }
            if (flags == 1 || flags == 0)
            {

                countGlobal++;

                if (flagH == 0)
                {
                    separatorInput(input_user);
                }
                else if (flagH == 1)
                {

                    separatorInput(guideInput);
                    flagH = 0;
                }

                if (strcmp("exit", input_user) == 0)
                {
                    exit(0);
                    break;
                }

                execParalel(arguments);
                memset(arguments, 0, sizeof(char) * (MAX_ARGS / 2 + 1));
            }
            else if (flags == 2)
            {

                PipeParallel();
                if (i > 2)
                {
                    // Se deixarmos o tamanho natural de I, o programa acaba lendo o NULL.
                    i = i - 1;
                    execParalel(arguments);
                }
            }
            else if (flags == 3)
            {

                readFileSeq();
                if (i > 2)
                {
                    i = i - 1;
                    execParalel(arguments);
                }
                memset(arguments, 0, sizeof(char) * (MAX_ARGS / 2 + 1));
            }
            else if (flags == 4)
            {

                recepOut();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
            else if (flags == 5)
            {

                append();
                if (i > 2)
                {
                    i = i - 1;
                    execPid(arguments);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    flags = 0;
    FILE *file;

    if (argc < 2)
    {

        int should_run = 1;
        flagChoose = 1;
        // flagChoose = chooseMode(input_initial);
        while (should_run)
        {
            if (should_run == 0)
            {
                break;
            }
            switch (flagChoose)
            {

            case 1:

                runInSequential();
                if (statusexit == 1)
                {
                    should_run = 0;
                    exit(0);
                }
                break;

            case 2:

                runInParallel();
                break;

            case 3:

                should_run = 0;
                break;

            default:

                printf("Digite um comando válido!\n");
            }
        }
    }
    else if (argc >= 2)
    {

        file = fopen(argv[1], "r");

        while (!feof(file) && !ferror(file))
        {
            argBatch[r] = malloc(sizeof *argBatch[r] * 41);
            fgets(argBatch[r], 41, file);
            argBatch[r][strlen(argBatch[r]) - 1] = '\0';
            r++;
        }

        for (int s = 0; s < r - 1; s++)
        {

           separgsBatch(argBatch);
           contBatch++;
        }
           if (flags == 0 || flags == 1)
           {

               execSeqBatch();
        }
        printf("%d", flags);
        printf("\nExec in Seq >\n");
        if (flags == 2)
        {

            printf("\ncheguei");
        }
        execParalBatch();
        //execSeqBatch();
        printf("\nExec in Parallel >\n");

       
         
        fclose(file);
    }

    return 0;
}