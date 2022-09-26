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
char *auxargs[MAX_ARGS / 2 + 1];
// Global variable to store user command
char input_user[BUFFER];
char guideInput[BUFFER];
char *history[BUFFER];
int countHistory = 0;
int flagH = 0;
// Global variable for count numbers of arguments
int argc = 0;
int statusexit = 0;
int count = 0;
int i;

void removeSpaces(char **str)
{
    while (**str == ' ')
        (*str)++;
}

void get_input()
{
    // get command from user
    printf("leam est seq> \n");
    fgets(input_user, BUFFER, stdin);
    
    if(input_user == 'h') {

        flagH = 1;
    }
 
    input_user[strlen(input_user) - 1] = '\0';
    strcpy(guideInput, input_user);

    /*if (guideInput[strlen(guideInput) - 1] == '\n')
    {

        guideInput[strlen(guideInput) - 1] = '\0';
    }*/
        if (strcmp(input_user, "exit") == 0)
        {
            printf("Ending our Shell application, GoodBye!\n");
            statusexit = 1;
        }
        // remove trailing newline       
    }

void separatorInput()
{
    // split string into argv
    char *delim = ";";
    char *delim2 = " ";
    char *separatorStr, *separatorStr2;
    i = 0;

    separatorStr = strtok(input_user, ";");
    
    while (separatorStr != NULL)
    {
        argv[i] = separatorStr;
        removeSpaces(&argv[i]);
        i++;      
        separatorStr = strtok(NULL, ";");
    }

    separatorStr2 = strtok(argv[count], " ");

    while(separatorStr2 != NULL) {
    
        auxargs[count] = separatorStr2;
        history[countHistory] = separatorStr2;
        countHistory++;
        count++;
        separatorStr2 = strtok(NULL, " ");
    }

    auxargs[count] = NULL;
    argv[i] = NULL;

    

    //Set the last position array to NULL, for the execvp syntax
    /*for (int j = 0; j < i; j++) {

        auxargs[count] = NULL;
    }*/
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

void execVp() {

    for (int j = 0; j < i; j++)
    {
        pid_t cpid[i];

        cpid[j] = fork();

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

        for (int k = 0; k < i; k++)
        {
            auxargs[k] = NULL;
        }
        
        //Decision for no segmantation fault
        if(j < (i-1)) {
        
        auxargs[0] = argv[j + 1];
        auxargs[i] = NULL;

        }
    }

    for (int m = 0; m < 5; m++)
    {

        argv[m] = NULL;
        i = 0;
        count = 0;
    }
}

void runInSequential()

{
    if(statusexit != 1) {

        while(1) {

            get_input();

            if(strcmp("exit", input_user) == 0) {
                
                break;

            }

            separatorInput();
            execVp();
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
    int flagChoose;

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

                //runInParallel();
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