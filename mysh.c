#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mysh.h"

int main() {
    char line[1024];
    Command cmd;

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        // Parse (Allocates memory)
        cmd = parse_command(line);

        // Safety Check
        if (cmd.command == NULL) {
            free_command(&cmd); // Clean up even if empty
            continue;
        }

        // Execute
        execute_command(cmd);
        
        // CLEANUP 
        // Free the memory. The child process has its own clone 
        // from fork(), and executor.c doesn't store pointers globally.
        free_command(&cmd);
    }

    return 0;
}