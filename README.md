# CMSC 125 Lab 1: UNIX SHELL

## Group Members
* Junel Arellano
* Julia Louise Contreras

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
    - The parent waits for the child process using
      `waitpid(pid, &status, 0)`. 
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
    - Output redirection: `echo hello > file.txt`
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
2. No command history.
3. No advanced signal group handling.
4. Output formatting may overlap with prompt during background completion.

## Design Decisions & Architecture

1. File Structure
```bash
├── Laboratory 1                  
│   ├── executor.c              # process creation and execution  
│   ├── Makefile                # build automation
│   ├── mysh.c                  # main shell loop
│   └── mysh.h                  # shared structures and definitions
│   └── parser.c                # command parsing
│   └── README.md
```
2. Execution flow
   1. User inputs command.
   2. Command is parsed into `Command` struct.
   3. `execute_command()` is called.
   4. `fork()` creates a child.
   5. Child processes:
      - Handles redirection
      - Resets signals (if foreground)
      - Calls `execvp()` 
   7. Parent processes:
      - Waits for foreground process to finish
      - Registes background jobs
     
3. Signal Strategy
   - `SIGINT` ignored by shell
   - Foregorund child resets to default (`SIG_DFL`)
   - `SIGCHLD` handler reaps background jobs
   - `setsid()` used for background process isolation
  
## Screenshots shows functionality
- Shell Running

  <img width="761" height="215" alt="image" src="https://github.com/user-attachments/assets/4d7616c7-fa4b-463e-ad40-2dca45cb31be" />
  
  - Command execution

    <img width="870" height="137" alt="image" src="https://github.com/user-attachments/assets/1f2db434-412f-4f0c-9a29-280388522a5e" />
    

- Background Job Example

  <img width="562" height="158" alt="image" src="https://github.com/user-attachments/assets/10c7f9f9-c5e8-4d76-b728-b7d865423215" />
  

- Ctrl+C Handling

  <img width="435" height="491" alt="image" src="https://github.com/user-attachments/assets/a9cc7ce9-e307-490d-bc5e-89f6786194c8" />


- Redirection Example
  - Input Redirection

    <img width="558" height="359" alt="image" src="https://github.com/user-attachments/assets/851b4eb0-5769-4a59-9e98-4e33588c189f" />


  - Output Redirection

    <img width="609" height="327" alt="image" src="https://github.com/user-attachments/assets/fa36edd4-8f5c-4829-bd74-4ec75ef0d602" />

    
  - Append Output

    <img width="584" height="351" alt="image" src="https://github.com/user-attachments/assets/9be89d7b-39ed-4c3e-afee-b77c5a563fdc" />

  
      
   
