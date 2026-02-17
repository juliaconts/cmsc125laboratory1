#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "mysh.h"

void execute_command(Command cmd)
{
    if (cmd.command == NULL)
        return;

    // Built-ins
    if (strcmp(cmd.command, "exit") == 0)
        exit(0);
    if (strcmp(cmd.command, "cd") == 0)
    {
        if (cmd.args[1])
            chdir(cmd.args[1]);
        return;
    }

    // External Command
    pid_t pid = fork();

    if (pid == 0)
    {
        // CHILD PROCESS
        // TODO:WEEK3 - redirection for open, dup2 must happen here
        if (cmd.background)
        {
            setsid();
        }

        // Input Redirection
        if (cmd.input_file)
        {
            int fd = open(cmd.input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("open input file");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Output Redirection
        if (cmd.output_file)
        {
            int flags = O_WRONLY | O_CREAT;
            flags |= cmd.append ? O_APPEND : O_TRUNC;
            int fd = open(cmd.output_file, flags, 0644);
            if (fd < 0)
            {
                perror("open output file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Execute command
        execvp(cmd.command, cmd.args);
        perror("mysh");
        exit(1);
    }
    else if (pid > 0)
    {
        // PARENT PROCESS

        // WEEK2 - no file redirection needed here (parent waits for the process)
        if (!cmd.background)
        {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0)
                {
                    printf("Command exited with code %d\n", exit_code);
                }
            }
        }
        else
        {
            // TODO:WEEK3 - put foreground job where parent does not wait
            static int job_id = 1;
            printf("[%d] Started PID: %d\n", job_id++, pid);
        }

        // TODO:WEEK3 - zombie reaping code goes here
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0)
        {
            // cleaned up one zombie
        }
    }
    else
    {
        // fork failure handling
        perror("fork failed");
    }
}