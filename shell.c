#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAX_ARGS 80
#define BUFFER 1024
#define READ_END
#define WRITE_END

int flags;
pid_t pidPipe[2];
char *argv[MAX_ARGS / 2 + 1];
char *auxargs[MAX_ARGS / 2 + 1];
int verEOF, verEOF2 = 0;
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
int i;

void removeSpaces(char **str) {

    while (**str == ' ')
        (*str)++;
}

void get_input() {
    // get command from user
    if(flagChoose == 1) {

        printf("leam seq> ");

    }else if(flagChoose == 2) {

        printf("leam par> ");
    }

    verEOF = fgets(input_user, BUFFER, stdin);

    input_user[strlen(input_user) - 1] = '\0';

    if(input_user == EOF) {

        flagChoose = 3;
        exit(0);
    }

    //Coloca os comandos no array auxiliar, caso não seja pressionado !!. Pois se for
    //Ele passa direto, logo, fica salvo o ultimo input antes do '!!'.
    if(strcmp(input_user, "!!") != 0) {

        strcpy(guideInput, input_user);
       
    }
    if (strcmp(input_user, "!!") == 0) {

        if (countGlobal == 0) {

            printf("No commands\n");
        }
        flagH = 1;
    }
    
    if (strcmp(input_user, "exit") == 0) {

        printf("Ending our Shell application, GoodBye!\n");
        statusexit = 1;
        exit(0);
    } 
}

void separatorArgv (char *argv[BUFFER]) {

    char *ptr;
    
    ptr = strtok(argv[countI], " ");

    while(ptr != NULL) {

        auxargs[count] = ptr;
        count++;
        ptr = strtok(NULL, " ");

    }
    auxargs[count] = NULL;
}

void separatorInput(char input_user[BUFFER]) {
    char *delimit = ";|>";
    char *verDelim= strdup(input_user);
    char *separatorStr;
    i = 0;

    separatorStr = strtok(input_user, delimit);

    if (verDelim[separatorStr - input_user + strlen(input_user)] == ';') {

        flags = 1;

    } else if (verDelim[separatorStr - input_user + strlen(input_user)] == '|') {

        flags = 2;

    } else if (verDelim[separatorStr - input_user + strlen(input_user)] == '>') {

        flags = 3;
    }
   
    while (separatorStr != NULL) {

        argv[i] = separatorStr;
        removeSpaces(&argv[i]);
        i++;
        separatorStr = strtok(NULL, delimit);
    }

    free(verDelim);
}

int chooseMode(char input_initial[30]) {

    printf("Choose mode for run Shell! style parallel or style sequential, '!!' to execute last command, and exit for endding!\n");
    verEOF2 = fgets(input_initial, 30, stdin);
    input_initial[strlen(input_initial) - 1] = '\0';
    if (input_initial == EOF) {

        flagChoose = 3;
        exit(0);
    }

    if(input_initial == NULL) {
        flagChoose = 3;
        exit(0);
    }
    if (input_initial[strlen(input_initial) - 1] == '\n') {

        input_initial[strlen(input_initial) - 1] = '\0';
    }
    if (strcmp(input_initial, "style sequential") == 0) {

        return 1;
    }
    else if (strcmp(input_initial, "style parallel") == 0) 
    {
        return 2;
    }
    else if (strcmp(input_initial, "exit") == 0) {

        printf("Ending our Shell application, GoodBye!\n");
        return 3;
        
    }
    else if (strcmp(input_initial, "!!") == 0) {

        printf("No commands\n");
        return 4;
    }
}

void execPid() {

    countGlobal++;

    if (flagH == 0){

        //separatorInput(input_user);

    } else if(flagH == 1) {

        separatorInput(guideInput);
        flagH = 0;

        if(i > 1) {

            i = i - 1;
        }
    }

    for (int j = 0; j < i; j++) {

        pid_t cpid[i];
        cpid[j] = fork();

        separatorArgv(&argv);

        if (cpid[j] < 0 ) {

            printf("Fork failed!\n");
        }
        else if (cpid[j] == 0) {

            execvp(auxargs[0], auxargs);
        }
        else {

            printf("Command: %s\n", auxargs[0]);
            wait(NULL);
        }     
        //Clear array
        for (int k = 0; k < i; k++) {

            auxargs[k] = NULL;     
        }
        countI++;
        count = 0;   
    }  
}

void runInSequential() {

    if(statusexit != 1) {

        while(1) {

            get_input();

            if (flagH == 1) {

                for (int l = 0; l < countI; l++) {

                    argv[l] = NULL;
                }
                //memset(argv, 0, MAX_ARGS / 2 + 1);
                countI = 0;
                execPid();
    
            }else if (flagH != 1) {
                for (int l = 0; l < countI; l++)
                {

                    argv[l] = NULL;
                }
                countI = 0;
            }

            if(strcmp("exit", input_user) == 0) {
                exit(0);
                break;
            }
            separatorInput(input_user);
            if(flags == 1 || flags == 0) {

                execPid();   

            }else if(flags == 2) {

                Pipe();

            }else if(flags == 3) {

                readFile();
            }
        }
    }
}

void runInParallel() {

    pid_t cpid[i];
    int j = 0;

    if (statusexit != 1) {

        while (1)
        {
            get_input();
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
           
            for (j = 0; j < i; j++)
            {             
                cpid[j] = fork();

                separatorArgv(&argv);

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

            while(countI > 0) {

                wait(NULL);
                countI--;
            }

            for (int m = 0; m < i; m++)
            {
                argv[m] = NULL;
            }           
        }
    }
}

void execPipe() {

    char write_msg[BUFFER];
    char read_msg[BUFFER];

    //separatorInput(input_user);
    separatorArgv(&argv);

    execvp(auxargs[0], auxargs);
  
}

void Pipe() {

    int fd[2];
    pidPipe[0] = fork();
   
    if (pidPipe < 0) {

        printf("Error!\n");

    }else if (pidPipe[0] == 0) {
        
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        separatorArgv(&argv);

        execvp(auxargs[0], auxargs);

    }else{

        wait(NULL);
    }

    for (int k = 0; k < i; k++) {

        auxargs[k] = NULL;
    }
    countI++;
    count = 0;
    pidPipe[1] = fork();

    if (pidPipe[1] < 0) {

        printf("Error!\n");

    } else if (pidPipe[1] == 0) {
        
        dup2(fd[0], STDIN_FILENO);
        separatorArgv(&argv);
        close(fd[1]);
        execvp(auxargs[0], auxargs);

    }else{
    
        wait(NULL);
    }

    close(fd[0]);
    close(fd[1]);
}

void readFile() {

    FILE *file;

    int fd = open("newfile.txt", O_CREAT | O_WRONLY, 0777);

    separatorArgv(&argv);

    pid_t pid1;

    pid1 = fork();
    close(STDOUT_FILENO);   
    dup2(fd, STDOUT_FILENO);
    close(fd);
    if(pid1 < 0) {

        printf("Error!\n");

    }else if(pid1 == 0) {

        execvp(auxargs[0], auxargs);
    
    }else {

        wait(NULL);
    } 
}

int main()
{
    int should_run = 1;

    flagChoose = chooseMode(input_initial);

    while (should_run)
    {
        //Trying resolve 'ctrl d' to end the file
        if(verEOF == EOF || verEOF2 == EOF) {

            should_run = 0;
            exit(0);
        }
        if(should_run == 0) {
            break;
        }
        switch(flagChoose) {

            case 1:

                runInSequential();
                if (statusexit == 1) {
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

            case 4:

                flagChoose = chooseMode(input_initial);
                break;

            default:

                printf("Opção inválida, tente novamente!\n");
                flagChoose = chooseMode(input_initial);
        }
    }

    return 0;
}