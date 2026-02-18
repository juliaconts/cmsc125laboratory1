#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "mysh.h"

// NEW: Helper function to free the memory we own
void free_command(Command *cmd) {
    // Free the command string if it exists
    if (cmd->command) {
        free(cmd->command);
        cmd->command = NULL;
    }

    // Free all arguments
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
        cmd->args[i] = NULL;
    }

    // Free filenames
    if (cmd->input_file) {
        free(cmd->input_file);
        cmd->input_file = NULL;
    }
    if (cmd->output_file) {
        free(cmd->output_file);
        cmd->output_file = NULL;
    }
}

Command parse_command(char input[]) {
    Command cmd;
    memset(&cmd, 0, sizeof(Command));

    // 1. Handle Empty Strings
    int is_empty = 1;
    for (int j = 0; input[j]; j++) {
        if (!isspace(input[j])) {
            is_empty = 0;
            break;
        }
    }
    if (is_empty) return cmd;

    // 2. Parse Tokens (Handling Quotes)
    int arg_idx = 0;
    int in_quote = 0;
    int in_token = 0;
    char *ptr = input;
    
    // Temporary array to hold pointers to the start of tokens
    char *temp_args[256]; 
    memset(temp_args, 0, sizeof(temp_args));

    while (*ptr != '\0' && arg_idx < 255) { 
        if (in_quote) {
            if (*ptr == '"') {
                in_quote = 0;
                *ptr = '\0'; // Terminate token
            }
        } 
        else {
            if (*ptr == '"') {
                in_quote = 1;
                if (!in_token) {
                    temp_args[arg_idx] = ptr + 1; // Start after quote
                    in_token = 1;
                }
            } 
            else if (isspace(*ptr)) {
                if (in_token) {
                    *ptr = '\0'; // Terminate previous token
                    in_token = 0;
                    arg_idx++;
                }
            } 
            else { 
                if (!in_token) {
                    if (*ptr == '&') {
                        cmd.background = true;
                        *ptr = '\0'; 
                    }
                    else {
                        temp_args[arg_idx] = ptr; // Start normal token
                        in_token = 1;
                    }
                }
            }
        }
        ptr++;
    }
    if (in_token) arg_idx++;

    // 3. Post-Processing: Copy strings to struct (The FIX for Ownership)
    int final_argc = 0;
    for (int i = 0; i < arg_idx; i++) {
        if (temp_args[i] == NULL) continue;

        if (strcmp(temp_args[i], ">") == 0) {
            if (i + 1 < arg_idx && temp_args[i+1]) {
                cmd.output_file = strdup(temp_args[i+1]); // strdup = OWNERSHIP
                cmd.append = false;
                i++;
            }
        }
        else if (strcmp(temp_args[i], ">>") == 0) {
            if (i + 1 < arg_idx && temp_args[i+1]) {
                cmd.output_file = strdup(temp_args[i+1]); 
                cmd.append = true;
                i++; 
            }
        }
        else if (strcmp(temp_args[i], "<") == 0) {
            if (i + 1 < arg_idx && temp_args[i+1]) {
                cmd.input_file = strdup(temp_args[i+1]); 
                i++;
            }
        }
        else {
            // Check if this is the command name
            if (final_argc == 0) {
                cmd.command = strdup(temp_args[i]);
            }
            // Always add to args list (Command needs to be args[0])
            cmd.args[final_argc++] = strdup(temp_args[i]); 
        }
    }
    cmd.args[final_argc] = NULL;

    return cmd;
}