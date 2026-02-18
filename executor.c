#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "mysh.h"

void sigchld_handler(int sig)
{
    (void)sig;

    int status;
    pid_t pid;

    // TODO:WEEK3 - zombie reaping code goes here
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("\n[Job PID %d] Completed\n", pid);
            fflush(stdout);
        }
        else if (WIFSIGNALED(status))
        {
            printf("\n[Job PID %d] Terminated\n", pid);
            fflush(stdout);
        }
    }
}

void execute_command(Command cmd)
{
    static int initialized = 0;

    // Initialize signals ONLY ONCE
    if (!initialized)
    {
        signal(SIGCHLD, sigchld_handler);
        signal(SIGINT, SIG_IGN);
        initialized = 1;
    }

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
            char cmd_str[1024] = "";

            for (int i = 0; cmd.args[i] != NULL; i++)
            {
                strcat(cmd_str, cmd.args[i]);
                if (cmd.args[i + 1] != NULL)
                {
                    strcat(cmd_str, " ");
                }
            }
            printf("[%d] Started background job: %s (PID: %d)\n", job_id++, cmd_str, pid);
        }
    }
    else
    {
        // fork failure handling
        perror("fork failed");
    }
}