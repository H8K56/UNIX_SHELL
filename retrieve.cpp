#include "retrieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Check if command exits
int is_builtin(const char* cmd) {
    char command[256];

    snprintf(command, sizeof(command), "which %s > /dev/null 2>&1", cmd);

    int result = system(command); 

    if (result == -1) {
        perror("system");
        return -1;
    }

    if (result == 0) {
        return 1; // Command exists
    }

    return 0;
}

int redirect_io(char* args[]){

}

int create_process(char* args[], int background){
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork error");
        return -1;
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Execution error");
            exit(EXIT_FAILURE);
        }
    } else {
        // background = 0 (foreground process) else (background process);
        if (!background) {
            wait(NULL);
        }
    }
    return 0;
}

void execute_command(char* args[]) {

}

void execute_pipeline(char* args1[], char* args2[]) {

}

void handle_builtin(char* args[]) {

}