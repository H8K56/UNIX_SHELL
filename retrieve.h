#ifndef RETRIEVE_H
#define RETRIEVE_H

void execute_command(char* args[]);
void execute_pipeline(char* args1[], char* args2[]);
void execute_parallel(char* args[]);
void handle_builtin(char* args[]);
int is_builtin(const char* cmd);
int redirect_io(char* args[]);
int create_process(char* args[], int background);


#endif