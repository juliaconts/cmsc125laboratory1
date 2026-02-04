#include <string.h>
#include <stdlib.h>
#include "mysh.h"

void parse_command(char *input, Command *cmd) {
    int i = 0;
    char *token = strtok(input, " \t\n");

    while (token != NULL && i < 255) {
        // Week 1: Detect background flag
        if (strcmp(token, "&") == 0) {
            cmd->background = true;
        }
        // Week 2: Redirection parsing will go here
        else {
            if (i == 0) cmd->command = token;
            cmd->args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    cmd->args[i] = NULL; // EXECVP needs a NULL terminator
}