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

    // TODO:WEEK3 - setting the & logic
    /* insert code here*/

    // External Command
    pid_t pid = fork();

    if (pid == 0)
    {
        // CHILD PROCESS
        // TODO:WEEK3 - redirection for open, dup2 must happen here
        /* insert code here */

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

        // TODO:WEEK3 - checks if backgroound job (&)
        /* insert code here */

        // WEEK2 - no file redirection needed here (parent waits for the process)
        if (!cmd.background)
        {
            waitpid(pid, NULL, 0);
        }
        else
        {
            // TODO:WEEK3 - put foreground job where parent does not wait
            printf("[%d] Started PID: %d\n", 1, pid);
        }

        // TODO:WEEK3 - zombie reaping code goes here
    }
    else
    {
        // fork failure handling
        perror("fork failed");
    }
}