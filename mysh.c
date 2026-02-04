#include <stdio.h>
#include <string.h>
#include "mysh.h"

int main() {
    char line[1024];
    Command cmd;

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        // Reset struct memory
        memset(&cmd, 0, sizeof(Command));

        parse_command(line, &cmd);
        execute_command(&cmd);
    }
    return 0;
}