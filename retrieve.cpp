#include "retrieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 100

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

int create_process(char* args[], int background) {
    pid_t pid = fork();
    int status;

    if (pid < 0) {
        perror("Fork error");
        return -1;
    } else if (pid == 0) {
        // In child process
        if (execvp(args[0], args) == -1) {
            perror("Execution error");
            exit(EXIT_FAILURE);
        }
    } else {
        if (!background) {
            waitpid(pid, &status, 0);
        } else {
            printf("Background process started with PID: %d\n", pid);
        }
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
    char* args1[MAX_INPUT_LENGTH] = {0};
    char* args2[MAX_INPUT_LENGTH] = {0};
    bool is_pipe = false;

    // this wiil check for parallel or pipe input
    for (int i = 0;args[i] != NULL;i++){
        args1[i] = args[i];
        if (strcmp(args[i],"&") == 0){
            // insert function for parallel execution
        }else if (strcmp(args[i],"|") == 0){
            is_pipe = true;
            args[i] = NULL;
            i++;
            // fill second half of the pipe
            for (int j = 0; args[i] != NULL; i++, j++){
                args2[j] = args[i];
            }
            break;
        }
    }

    if (is_pipe) {
        execute_pipeline(args1, args2);
    } else {
        create_process(args1,0);
    }
}

void display_input(char* args[]) {
    printf("Command: %s\n", args[0]);
    for (int i = 1; args[i] != NULL; i++) {
        printf("Argument %d: %s\n", i, args[i]);
    }
}