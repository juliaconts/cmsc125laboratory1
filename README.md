# CMSC 125 Lab 1: UNIX SHELL

## Group Members
* Junel Arellano
* Julia Contreras

## Description
A simple Unix Shell implementation using POSIX API where it supports command execution, background jobs, signal handling, and input/output redirection. The shell mimics basic behavior of the standard UNIX shell.

## Compilation and Usage Instructions

1. Compilation
    - Make sure to have `gcc` installed.
    - Run `make` to compile.
    - To remove compiled files for rebuilding the project use `make clean` to delete object files (*.o) and executable files (mysh)
    - To run again after deleting you need to run `make all` to explicitly compile all source files into executable.

2. Running the Shell
    - Type the command `./mysh` to activate the shell and see something like this: `mysh>`
    
## Implemented Features

1. Foreground Execution
    - It commands without `&` run in the foreground
    - This shell creates a child process using `fork()` and executes the command using `execvp()`.
    - The parent waits for the child process using `waitpid(pid, &status, 0)`. 
    - The shell does not display a new prompt until the process finishes.
    - If the process is terminated by `Ctrl+C` (SIGINT), the shell displays:
    `[Process <PID> killed by signal #]`

2. Command Execution
    - Executes external programs using `fork()` and `execvp()`.
    - Parent waits foreground processes.

3. Built-in Commands
    - `cd <directory>` for changing directory
    - `exit` to terminate the shell

4. Background Execution (&)
    - This uses `&` to run commands in the background.
    - This is the part where the shell does not wait and displays the jobID and PID.
    Example: 
    `sleep 10 &`
        - Background jobs are stored in a job table that displays:
            `[#] Started background job: sleep 10 (PID: #####)`
        - This will automatically reaped using `SIGCHLD` as it displays when finished:
            `[#] Done (PID #####)`

5. Input and Output Redirection
    - Input redirection: `cat < input.txt`
    - Output redirection: `echo helllo > file.txt`
    - Append output: `echo world >> file.txt`
    - Uses `open()`, `dup2()`, and `close()`


6. Zombie Process Reaping
    - This uses the `SIGCHLD` handler where it will automatically clean terminated child processes.
    - This prevents proccesses where the parent never collected the exit status of the child making it a zombie process.

7. Ctrl+C Handling (SIGINT)
    - The shell ignores Ctrl+C where it will only terminate a foreground process.
    - Displays:
    `[Process 12345 killed by signal 1]`

## Known Limitations/Bugs
1. No support for:
    - Pipes (|)
    - Command chaining (&&, ||)
    - Job control commands (jobs, fg, bg)
2. No support for multiple redirections in one command.
3. No command history.
4. No advanced signal group handling.
5. Output formatting may overlap with prompt during background completion.

## Design Decisions & Architecture
