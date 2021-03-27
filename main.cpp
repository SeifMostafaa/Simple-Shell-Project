#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

using namespace std;


//Initializing the shell

void init_shell(){

    char* username = getenv("USER");

    clear();
    printf("\n\n******************************\n\n");
    printf("*****\t*****\t*****\t*****\n");
    printf("*  \t*  \t  * \t*\n");
    printf("*  \t*  \t  * \t*\n");
    printf("*****\t*****\t  * \t*****\n");
    printf("    *\t*  \t  * \t*\n");
    printf("    *\t*  \t  * \t*\n");
    printf("*****\t*****\t*****\t*\n");
    printf("\n******************************\n\n");
    printf("\n 'WE WORK IN THE DARK TO SERVE THE LIGHT' <) :))\n");
    printf("\n USER is: @%s\n", username);
    printf("\n\n******************************\n\n");

    sleep(2);
    clear();


}


//Scans input from the user and pass it to the arguments array

int take_input(char* arr){
    char* input = readline("\n>>> ");

    if (input != NULL){
        add_history(input); //To save the commands to be able to navigate by arrows
        strcpy(arr, input);
        return 0;
    }

    return 1;
}


//Printing the current directory

void print_dir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s\n", cwd);
}


void handler(int sig){
    FILE* file = fopen("/home/seif/Desktop/ShellProjectNew/log.txt", "a");
    fputs("Child Terminated \n", file);
    fclose(file);
}

void handleFirefox(int sig){
    wait(NULL);
}



void execute_args(char** args)
{
    signal(SIGCHLD, handler);

    // Forking a child
    int pid = fork();

    if (pid == -1) {

        //Creation of child process was unsuccessful
        printf("\nForking child was unsuccessful");
        return;
    } else if (pid == 0) {

            //New child process has been created
        if (execvp(args[0], args) < 0) {
            //This condition will be true if the execvp returns a negative value when the execution fails (e.g., the request file does not exist).
            printf("\nCould not execute the command");
        }
        exit(0);
    } else {

        if (args[1] != NULL){
            if (strcmp(args[1], "&") == 0){
                signal(SIGCHLD, handleFirefox);
                return;
                }

            }

        // The child is in process and waiting for it to terminate
        wait(NULL);
        return;
    }


}


// Function to execute builtin commands
int cmdHandler(char** args)
{
    int NoOfOwnCmds = 2, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";


    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(args[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
    case 1:
        printf("\nGoodbye!\n");
        exit(0);
    case 2:
        chdir(args[1]);
        return 1;
    default:
        break;
    }

    return 0;
}


// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed)
{
    parseSpace(str, parsed);

    return cmdHandler(parsed)? 0 : 1;
}

int main()
{

char inputString[MAXCOM], *parsedArgs[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1) {
        // print shell line
        print_dir();
        // take input
        if (take_input(inputString))
            continue;
        // process

        // 1 if it is a simple command
        // execute
        if (processString(inputString, parsedArgs))
            execute_args(parsedArgs);



    }
    return 0;
}



