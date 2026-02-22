#ifndef MYSH_H
#define MYSH_H

#include <stdbool.h>
#include <sys/types.h> 

typedef struct {
    char *command;          // program name
    char *args[256];        // argument list needed for execcvp
    char *input_file;     
    char *output_file;
    bool append;
    bool background;        // flag set to true if user typed &
} Command;

#define MAX_JOBS 64

typedef struct {
    pid_t pid;
    int job_id;
    char cmd_str[256];
} Job;

extern Job job_table[MAX_JOBS];
extern int job_count;

// Function prototypes
Command parse_command(char input[]);
void execute_command(Command cmd);

// New: Function to clean up memory after execution
void free_command(Command *cmd);

#endif