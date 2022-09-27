#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_ARGS 80
#define BUFFER 1024
#define READ_END
#define WRITE_END
// Global variable for store the arguments
char *argv[MAX_ARGS / 2 + 1];
int countI = 0;
char *auxargs[MAX_ARGS / 2 + 1];
// Global variable to store user command
char input_user[BUFFER];
char guideInput[BUFFER];
char *history[BUFFER];
int countHistory = 0;
int flagChoose;
int flagH = 0;
// Global variable for count numbers of arguments
int argc = 0;
int statusexit = 0;
int count = 0;
int i;

void checkChar(str) {

    int exclamationCheck, ptvgla, file;

    if (strchr(str, '|') != NULL)
    {
        exclamationCheck = 1;

    }else if (strchr(str, ";") != NULL) {

        ptvgla = 1;

    }else if (strchr(str, ">") != NULL) {

        file = 1;
    }
}

void removeSpaces(char **str)
{
    while (**str == ' ')
        (*str)++;
}

void get_input()
{
    // get command from user
    if(flagChoose == 1) {

        printf("leam seq> ");

    }else if(flagChoose == 2) {

        printf("leam par> ");
    }

    fgets(input_user, BUFFER, stdin);

    input_user[strlen(input_user) - 1] = '\0';
    guideInput[strlen(guideInput) - 1] = '\0';

    if(strcmp(input_user, "h") != 0) {

        strcpy(guideInput, input_user);
    }
    if (strcmp(input_user, "h") == 0)
    {
        flagH = 1;
    }
    
    if (strcmp(input_user, "exit") == 0)
    {
        printf("Ending our Shell application, GoodBye!\n");
        statusexit = 1;
    }
        // remove trailing newline       
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

void separatorInput(char input_user[BUFFER])
{
    // split string into argv
    char *delim = ";";
    char *delim2 = " ";
    char *separatorStr, *separatorStr2;
    i = 0;
    //Parse string in to numbers of arguments
    separatorStr = strtok(input_user, ";");
    
    while (separatorStr != NULL)
    {
        argv[i] = separatorStr;
        removeSpaces(&argv[i]);
        i++;      
        separatorStr = strtok(NULL, ";");
    }
  
}

int chooseMode(char input_initial[30]) {

    printf("Choose mode for run Shell! style parallel or style sequential, h to view last command, and exit for endding!\n");
    fgets(input_initial, 30, stdin);

    if (input_initial[strlen(input_initial) - 1] == '\n')
    {
        input_initial[strlen(input_initial) - 1] = '\0';
    }
    if (strcmp(input_initial, "style sequential") == 0)
    {
        return 1;
    }
    else if (strcmp(input_initial, "style parallel") == 0) 
    {
        return 2;
    }
    else if (strcmp(input_initial, "exit") == 0)
    {
        printf("Ending our Shell application, GoodBye!\n");
        return 3;
        
    }
}

void execPid() {

    if(flagH == 0) {

        separatorInput(input_user);
        
    }

    if(flagH == 1) {

        separatorInput(guideInput);
        flagH = 0;
    }

    for (int j = 0; j < i; j++)
    {
        pid_t cpid[i];

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
        else
        {
            printf("Command: %s\n", auxargs[0]);
            wait(NULL);
        }     
        //Clear array
        for (int k = 0; k < i; k++)
        {
            auxargs[k] = NULL;
            
        }
        countI++;
        count = 0;   
    }  
}

void runInSequential()
{
    if(statusexit != 1) {

        while(1) {

            get_input();

            if (flagH == 1) {

                memset(argv, 0, MAX_ARGS / 2 + 1);
                countI = 0;
                execPid();
    
            }

            else if (flagH != 1)
            {
                memset(argv, 0, MAX_ARGS / 2 + 1);
                countI = 0;
                // i = 0;
                // count = 0;
            }

            if(strcmp("exit", input_user) == 0) {
                
                break;

            }         
            execPid();         
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
            separatorInput(input_user);

            if (strcmp("exit", input_user) == 0)
            {
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
                // Decision for no segmantation fault               
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

void execPipe()
{

    char write_msg[BUFFER];
    char read_msg[BUFFER];
    int fd[2];
    pid_t pip1, pip2;
    
}

int main()
{
    int should_run = 1;
    char input_initial[30];
    
    flagChoose = chooseMode(input_initial);

    while (should_run)
    {
        
        switch(flagChoose) {

            case 1:

                runInSequential();
                if (statusexit == 1) {
                    should_run = 0;
                }
                break;

            case 2:

                runInParallel();
                break;

            case 3:

                should_run = 0;
                break;

            default:

                printf("Opção inválida, tente novamente!\n");
                flagChoose = chooseMode(input_initial);
        }
    }

    return 0;
}