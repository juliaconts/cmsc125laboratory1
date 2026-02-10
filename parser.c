#include <string.h>
#include <stdlib.h>
#include "mysh.h"

Command parse_command(char input[]) {

    // Local variable creation
    Command cmd;  

    memset(&cmd, 0, sizeof(Command));

    int i = 0;

    // TODO:Week3 - edge case handling for whitespaces will go here
    /* insert code here*/

    char *token = strtok(input, " \t\n");

    // TODO:Week3 - memory safety to limit no of args to avoid overflow
    /* insert code here*/

    while (token != NULL && i < 255)
    {

        // TODO:Week3 - edge case handling for validation of empty strings
        /* insert code here*/

        // Week1 - Detect background flag
        if (strcmp(token, "&") == 0) {
            cmd.background = true;
        }
        else {
            // TODO:Week3 - memory safety
            /* insert code here*/

            if (i == 0) {
                cmd.command = token;
            }
            cmd.args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    cmd.args[i] = NULL; // EXECVP needs a NULL terminator

    return cmd
}