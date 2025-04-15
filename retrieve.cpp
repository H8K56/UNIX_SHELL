#include "retrieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


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

}

void execute_command(char* args[]) {

}

void execute_pipeline(char* args1[], char* args2[]) {

}

void handle_builtin(char* args[]) {

}