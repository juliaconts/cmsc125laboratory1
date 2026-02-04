#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "mysh.h"

void execute_command(Command *cmd)
{
    if (cmd->command == NULL)
        return;

    // Built-ins
    if (strcmp(cmd->command, "exit") == 0)
        exit(0);
    if (strcmp(cmd->command, "cd") == 0)
    {
        if (cmd->args[1])
            chdir(cmd->args[1]);
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

        // TODO:WEEK2 - file redirection handling should be done here
        /* insert code here */
        execvp(cmd->command, cmd->args);
        perror("mysh");
        exit(1);
    }
    else if (pid > 0)
    {
        // PARENT PROCESS

        // TODO:WEEK3 - checks if backgroound job (&)
        /* insert code here */

        // TODO:WEEK2 - no file redirection needed here (parent waits for the process)
        /* insert code here*/
        if (!cmd->background)
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