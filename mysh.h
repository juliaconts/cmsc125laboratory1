#ifndef MYSH_H
#define MYSH_H

#include <stdbool.h>

typedef struct {
    char *command;
    char *args[256];
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

void parse_command(char *input, Command *cmd);
void execute_command(Command *cmd);

#endif