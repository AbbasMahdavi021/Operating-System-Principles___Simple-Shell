/***********************************************************
* Class:  CSC-415-03 Fall 2022
* Name: Abbas Mahdavi
* Student ID: 918345420
* GitHub Name: AbbasMahdavi021
* Project: Assignment 3 – Simple Shell
*
* File: Mahdavi_Abbas_HW3_main.c
*
* Description: 
* The focus of this assignment is to program and develop 
* a shell that runs within the linux terminal. The shell 
* takes in user input and text files like a normal prompt
* and produces the result after entering it. The buffer
* limit on this assignment is 123 bytes and uses the fork()
* function to create a separate child process to take over
* the program and overtake the next instruction.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#define BUFFERSIZE 123	//123 byte buffer size
#include <unistd.h>
//https://en.wikipedia.org/wiki/Unistd.h

void inputCMD(char cmd[], char * tempTok[]){
    int position;
    char* tokenizer;
    if(fgets(cmd, BUFFERSIZE, stdin)!=NULL) { 
      //stdin allows file read and user input

        size_t length = strlen(cmd); 
        // Size of the the read/input string

        if (length == 1 || 0){
            printf("%s", ">\n");
        }

        tokenizer = strtok(cmd, " "); 
        // tokenize the string using space delimiter
        if (cmd[length - 1] == '\n') {
          //break to next line
            cmd[length - 1] = '\0'; 
            // null termination
        }
        position = 0; //set index position of iterator
        while (tokenizer != NULL) {
            tempTok[position] = tokenizer;
            //assign position of the token to tempTok while its not null
            // and then set token to null using space delimeter
            tokenizer = strtok(NULL, " ");
            position++;
        }
        tempTok[position] = NULL;
    }else {
    
        exit(0); //endpoint
    }
}

int main(int argc, char* argv[]){

    char* source = "/bin/"; //path variable for execvp()
    char pathHold[20];
    char* argHold[123];
    char commandLine[BUFFERSIZE]; //entire command line that the user inputs
    pid_t kid; // Child ID
    int exiting; // Exit status


    while(1) { // Infinet loop until the user type "exit"
        //will take in user input
        printf("prompt> ");
        inputCMD(commandLine,argHold);
        // if the input "exit" then break the loop
        if( strcmp(commandLine, "exit") == 0)
        { //checking if buffer input equals 'exit' when read using strcmp
            break;
        }

            int processRender = fork();
        if (processRender == 0){ // fork to child process
            //Child
            strcpy(pathHold, source); // copy "/bin/" to full path
            strcat(pathHold, argHold[0]); // combine
            if(execvp(pathHold, argHold) == -1){ // if execvp fails
                perror("Error in child process\n");
                exit(2);
            }

        } else {
            //parent scope
            kid = wait(&exiting);
            if (WIFEXITED(exiting)) {
                // print child ID and return status

                //http://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/notes/Exec/lecture.html
                //source for WEXITSTATUS implementation
                kid = getpid();
                int statusExit = WEXITSTATUS(exiting);
            printf("Child %d, exited with %d\n", kid, statusExit);
            }
        }
    }
    return 0; //exit main
}