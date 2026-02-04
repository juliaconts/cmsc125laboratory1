#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "mysh.h"

void execute_command(Command *cmd) {
    if (cmd->command == NULL) return;

    // Built-ins
    if (strcmp(cmd->command, "exit") == 0) exit(0);
    if (strcmp(cmd->command, "cd") == 0) {
        if (cmd->args[1]) chdir(cmd->args[1]);
        return;
    }

    // External Command
    pid_t pid = fork();

    if (pid == 0) {
        // CHILD PROCESS
        execvp(cmd->command, cmd->args);
        perror("mysh"); 
        exit(1);
    } else if (pid > 0) {
        // PARENT PROCESS
        if (!cmd->background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("[%d] Started PID: %d\n", 1, pid);
        }
    } else {
        perror("fork failed");
    }
}