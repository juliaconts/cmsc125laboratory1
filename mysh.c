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
        // If it is NOT a background command, we clean it up immediately.
        // If it IS a background command, we leave the memory alive so 
        // the executor can track it in the Job List.
        if (!cmd.background) {
            free_command(&cmd);
        }
    }

    return 0;
}