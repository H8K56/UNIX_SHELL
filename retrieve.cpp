#include "retrieve.h"

// Check if command exits

/**
 * @brief Checks if a given command is a built-in command or available in the system's PATH.
 *
 * This function uses the `which` command to determine if the specified command
 * exists in the system's PATH. It executes the `which` command via the `system` function
 * and checks the result.
 *
 * @param cmd A C-string representing the command to check.
 *            It should not be null and must point to a valid null-terminated string.
 *
 * @return Returns 1 if the command is found (indicating it is built-in or available),
 *         0 if the command is not found, and -1 if an error occurs while executing the `system` call.
 */
int CommandExecutor::is_builtin(const char* cmd) {
    char command[256];
    snprintf(command, sizeof(command), "which %s > /dev/null 2>&1", cmd);
    int result = system(command);

    if (result == -1) {
        perror("system");
        return -1;
    }
    return result == 0;
}

/**
 * @brief Redirects input or output based on the specified arguments.
 *
 * This function handles input/output redirection for a command by modifying
 * the standard input (STDIN) or standard output (STDOUT) file descriptors.
 * It supports redirection operators `>` for output redirection and `<` for
 * input redirection.
 *
 * @param args An array of C-style strings representing the command and its
 *             arguments. The redirection operator (e.g., `>` or `<`) is
 *             expected at `args[1]`, and the target file path is expected
 *             at `args[2]`.
 *
 * @return Returns 0 on success, or -1 if an error occurs (e.g., file opening
 *         fails). In case of an error, an appropriate error message is printed
 *         using `perror`.
 */
int CommandExecutor::redirect_io(char* args[]) {
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

/**
 * @brief Creates a new process to execute a command.
 *
 * This function forks the current process and executes the specified command
 * using `execvp`. It handles both foreground and background execution based on
 * the `background` parameter.
 *
 * @param args An array of C-style strings representing the command and its arguments.
 *             The first element should be the command name, followed by its arguments.
 *
 * @param background An integer indicating whether to run the command in the background (1)
 *                   or foreground (0). If set to 1, the function does not wait for
 *                   the child process to finish.
 *
 * @return Returns 0 on success, or -1 if an error occurs during process creation or execution.
 */
int CommandExecutor::create_process(char* args[], int background) {
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
    } else if (!background) {
        waitpid(pid, &status, 0);
    } else {
        printf("Background process started with PID: %d\n", pid);
    }
    return 0;
}

/**
 * @brief Executes a command based on the provided arguments.
 *
 * This function checks if the command is built-in or available in the system's PATH.
 * If it is a built-in command, it handles it accordingly. Otherwise, it creates a new
 * process to execute the command.
 *
 * @param args An array of C-style strings representing the command and its arguments.
 *             The first element should be the command name, followed by its arguments.
 */
void CommandExecutor::execute_command(char* args[]) {
    if (args[0] == NULL) {
        fprintf(stderr, "No command entered\n");
        return;
    }
    if (is_builtin(args[0])) {
        handle_builtin(args);
    } else {
        printf("Command does not exist\n");
    }
}

void CommandExecutor::execute_pipeline(char* args1[], char* args2[]) {
    execute_pipeline_internal(args1, args2);
}

void CommandExecutor::execute_parallel(char* args[]) {
    execute_parallel_internal(args);
}

void CommandExecutor::execute_parallel_internal(char* args[]) {
    char* current_command[MAX_INPUT_LENGTH] = {0};
    int arg_index = 0;

    for (int i = 0; i < MAX_INPUT_LENGTH && args[i] != NULL; i++) {
        if (strcmp(args[i], "&") == 0) {
            current_command[arg_index] = NULL;
            create_process(current_command, 1);
            arg_index = 0;
        } else {
            current_command[arg_index++] = args[i];
        }
    }

    if (arg_index > 0) {
        current_command[arg_index] = NULL;
        create_process(current_command, 1);
    }
}



void CommandExecutor::execute_pipeline_internal(char* args1[], char* args2[]) {
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("Pipe error");
        return;
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("Fork error");
        return;
    }

    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args1[0], args1);
        perror("Execution error of first command");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork error");
        return;
    }

    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args2[0], args2);
        perror("Execution error of second command");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


void CommandExecutor::handle_builtin(char* args[]) {
    char* args1[MAX_INPUT_LENGTH] = {0};
    char* args2[MAX_INPUT_LENGTH] = {0};
    bool is_pipe = false, is_parallel = false;

    for (int i = 0; args[i] != NULL; i++) {
        args1[i] = args[i];
        if (strcmp(args[i], "&") == 0) {
            is_parallel = true;
            break;
        } else if (strcmp(args[i], "|") == 0) {
            is_pipe = true;
            args[i] = NULL;
            i++;
            for (int j = 0; args[i] != NULL; i++, j++) {
                args2[j] = args[i];
            }
            break;
        }
    }

    if (is_parallel) {
        execute_parallel(args);
    } else if (is_pipe) {
        execute_pipeline(args1, args2);
    } else {
        create_process(args, 0);
    }
}