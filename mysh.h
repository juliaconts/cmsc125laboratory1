#ifndef MYSH_H
#define MYSH_H

#include <stdbool.h>

typedef struct {
    char *command;          // program name
    char *args[256];        // argument list needed for execcvp
    char *input_file;     
    char *output_file;
    bool append;
    bool background;        // flag set to true if user typed &
} Command;

// Function prototypes
Command parse_command(char input[]);
void execute_command(Command cmd);

#endif