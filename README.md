# UNIX Shell

This project provides a simple implementation of a UNIX shell. The shell supports basic commands such as `parallel`, `cd`, `ls`, `pipeline`, and `command check`.

## Features

### 1. `parallel '&'`
- Executes multiple commands in parallel.
- Syntax: `parallel <command1> : <command2> : ...`

### 2. `cd`
- Changes the current working directory.
- Syntax: `cd <directory_path>`

### 3. `ls`
- Lists files and directories in the current directory.
- Syntax: `ls`

### 4. `pipeline '|'`
- Executes a series of commands where the output of one command is passed as input to the next.
- Syntax: `<command1> | <command2> | ...`

### 5. `command check`
- Verifies if a command is valid and executable.
- Syntax: `command check <command>`

## Instructions to Run

1. Clone the repository:
    ```bash
    git clone <repository_url>
    cd UNIX_SHELL
    ```

2. Compile the code:
    ```bash
    g++ -o unix_shell main.cpp
    ```

3. Run the shell:
    ```bash
    ./unix_shell
    ```

4. Use the supported commands as described above.

## Example Usage

- Run commands in parallel:
  ```bash
  parallel; ls & pwd & echo "Hello, World!"
  ```

- Use a pipeline:
  ```bash
  ls | grep "file"
  ```

- Check a command:
  ```bash
  command check ls
  ```