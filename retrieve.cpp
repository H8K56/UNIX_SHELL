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
    if (strcmp(args[1], ">") == 0) {
        FILE* file = fopen(args[2], "w");
        if (file == NULL) {
            perror("File opening error");
            return -1;
        }
        dup2(fileno(file), STDOUT_FILENO);
        fclose(file);
        args[1] = NULL;
    } else if (strcmp(args[1], "<") == 0) {
        FILE* file = fopen(args[2], "r");
        if (file == NULL) {
            perror("File opening error");
            return -1;
        }
        dup2(fileno(file), STDIN_FILENO);
        fclose(file);
        args[1] = NULL;
    }
    return 0;
}

int create_process(char* args[], int background = 0){
    pid_t pid = fork();
    int status;

    if (pid < 0) {
        perror("Fork error");
        return -1;
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Execution error");
            exit(EXIT_FAILURE);
        }
    } else if(pid > 0){
        // // background = 0 (foreground process) else (background process);
        // if (!background) {
        //     wait(NULL);
        // }
        wait(&status);
    }
    return 0;
}

void execute_command(char* args[]) {
    if (args[0] == NULL) {
        fprintf(stderr, "No command entered\n");
        return; // No command entered
    }

    if(is_builtin(args[0]) == 1) {
        handle_builtin(args);
        return;
    }else {
        printf("Command does not exist\n");
    }
}

void execute_pipeline(char* args1[], char* args2[]) {
    return;
}

void handle_builtin(char* args[]) {
    create_process(args, 0);
    return;
}

void display_input(char* args[]) {
    printf("Command: %s\n", args[0]);
    for (int i = 1; args[i] != NULL; i++) {
        printf("Argument %d: %s\n", i, args[i]);
    }
}