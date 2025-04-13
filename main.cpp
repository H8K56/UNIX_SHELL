#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>

#define MAX_INPUT_LENGTH 100

int main(){
    char input[MAX_INPUT_LENGTH];
    char* args[MAX_INPUT_LENGTH];
    int status;

    while(true){
        std::cout << "wish> ";
        std::cin.getline(input, sizeof(input)); // user input

        char* input_ptr = input;

        /*Tokenize the input*/
        char* token = strsep(&input_ptr, " \n");
        int i = 0;

        while(token != NULL && i < MAX_INPUT_LENGTH){
            args[i] = token;
            token = strsep(&input_ptr, " \n");
            i++;
        }
        args[i] = NULL; /*Null-terminate the argument list*/
        
        if(strcmp(args[0], "exit") == 0){
            std::cout << "Exiting the shell...\n";
            break;
        }
    }
}