#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include "retrieve.cpp"

#define MAX_INPUT_LENGTH 100

int main() {
    char input[MAX_INPUT_LENGTH];
    char* args[MAX_INPUT_LENGTH];

    CommandExecutor executor;

    while (true) {
        std::cout << "wish> ";
        if (!std::cin.getline(input, MAX_INPUT_LENGTH)) {
            std::cout << "\n";
            break; // EOF
        }

        // Tokenize input into args
        char* input_ptr = input;
        char* token;
        int i = 0;

        while ((token = strsep(&input_ptr, " \t")) != NULL) {
            if (*token == '\0') continue;
            args[i++] = token;
        }
        args[i] = NULL;

        if (i == 0) continue;

        if (strcmp(args[0], "exit") == 0) {
            std::cout << "Exiting the shell...\n";
            break;
        }

        executor.handle_builtin(args);
    }

    return 0;
}
