#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_ARGS 80
#define BUFFER 1024
// Global variable for store the arguments
char *argv[MAX_ARGS / 2 + 1];
// Global variable to store user command
char input_user[BUFFER];
char guideInput[BUFFER];
int statusFlag = 1;
// Global variable for count numbers of arguments
int argc = 0;
int count = 0;
char i;

int verifyExit() {

    if(strcmp(guideInput, "exit") == 0) {

        return 0;

    }
}

void get_input()
{
    // get command from user
    printf("leam est seq> \n");
    fgets(input_user, BUFFER, stdin);
    strcpy(guideInput, input_user);

    if (guideInput[strlen(guideInput) - 1] == '\n')
    {

        guideInput[strlen(guideInput) - 1] = '\0';
    }
        if (strcmp(guideInput, "exit") == 0)
        {
            printf("Ending our Shell application, GoodBye!\n");
            statusFlag = verifyExit();
            exit(0);
                }
        // remove trailing newline
        if (input_user[strlen(input_user) - 1] == '\n')
        {

            input_user[strlen(input_user) - 1] = '\0';
        } // printf("%s\n", cmd);
    }

void separatorInput()
{
    // split string into argv
    char *separatorStr;
    i = 0;
    separatorStr = strtok(input_user, "; ");
    while (separatorStr != NULL)
    {
        // printf("%s\n", ptr);
        argv[i] = separatorStr;
        i++;
        separatorStr = strtok(NULL, "; ");
    }

    /* check for "; "
    if (!strcmp("; ", argv[i - 1]))
    {
        argv[i - 1] = NULL;
        argv[i] = "; ";
    }*/
    
    //Set the last position array to NULL, for the execvp syntax
    argv[i] = NULL;
    
    //Set the first position to array to NULL
    guideInput[0] = '\0';
    // printf("%d\n", i);
}

int chooseMode(char input_initial[30]) {

    printf("Choose mode for run Shell! style parallel or style sequential, exit for endding!\n");
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
        exit(0);
    }
}

void runShell()
{
    get_input();
    separatorInput();
}

void runInSequential()
{
    pid_t pid, wpid;
    int status;

    runShell();

    while(statusFlag != 0) {

        if(statusFlag == 0) {

            break;
        }

        pid = fork();

        if(pid < 0) {

            exit(1);

        }else if(pid == 0) {

            execvp(argv[0], argv);
        }else {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        break;
    }
}
void runInParallel()
{

    pid_t pid_Ch[MAX_ARGS / 2 + 1];
    pid_t pid;
    int count = 0;

    while (1)
    {
        runShell();

        
        pid = fork();
        pid_Ch[count] = pid;

        while (argc >= 0)
        {

            if (pid < 0)
            {

                printf("Fork failed!\n");
                exit(EXIT_FAILURE);
                exit(1);
            }
            else if (pid == 0)
            {

                execvp(argv[0], argv);
            }
            else
            {

                printf("Waiting Child terminate de process first!\n");
                wait(NULL);
            }

            argc--;
        }
    }
}

void execPipe() {

    int dup2[2];
    pid_t pip1, pip2;

    
}

int main()
{
    int should_run = 1;
    char input_initial[30];
    int flagChoose;

    
    while (should_run)
    {
        flagChoose = chooseMode(input_initial);

        switch(flagChoose) {

            case 1:

                runInSequential();
                break;

            case 2:

                runInParallel();
                break;

            case 3:

                should_run = 0;
                break;

            default:

                printf("Opção inválida, tente novamente!\n");
                
            }
    }

    return 0;
}