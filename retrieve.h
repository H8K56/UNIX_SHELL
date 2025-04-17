#ifndef RETRIEVE_H
#define RETRIEVE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

class CommandExecutor {
private:
    static const int MAX_INPUT_LENGTH = 100;

    int is_builtin(const char* cmd);
    int redirect_io(char* args[]);
    int create_process(char* args[], int background);
    // I kept these private in accordance with defensive programming
    void execute_pipeline_internal(char* args1[], char* args2[]);
    void execute_parallel_internal(char* args[]); 

public:
    void execute_command(char* args[]);
    void execute_pipeline(char* args1[], char* args2[]);
    void execute_parallel(char* args[]);
    void handle_builtin(char* args[]);
};

#endif