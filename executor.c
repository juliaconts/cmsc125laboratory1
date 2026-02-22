#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "mysh.h"


Job job_table[MAX_JOBS];
int job_count = 0;
static int next_job_id = 1;

void add_job(pid_t pid, const char *cmd_str) {
    if (job_count >= MAX_JOBS) return;
    job_table[job_count].pid = pid;
    job_table[job_count].job_id = next_job_id++;
    strncpy(job_table[job_count].cmd_str, cmd_str, 255);
    job_table[job_count].cmd_str[255] = '\0';
    job_count++;
}

int remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (job_table[i].pid == pid) {
            int jid = job_table[i].job_id;
            for (int j = i; j < job_count - 1; j++)
                job_table[j] = job_table[j + 1];
            job_count--;
            return jid;
        }
    }
    return -1;
}

// SIGCHILD handler for background jobs
void sigchld_handler(int sig)
{
    (void)sig;

    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        int jid = remove_job(pid);
        if (jid < 0) continue; // wasn't a tracked background job

        char buf[128];
        int n;
        if (WIFEXITED(status))
            n = snprintf(buf, sizeof(buf), "\n[%d] Done (PID %d)\n", jid, pid);
        else
            n = snprintf(buf, sizeof(buf), "\n[%d] Terminated (PID %d)\n", jid, pid);
        
        write(STDOUT_FILENO, buf, n);
    }
}

void execute_command(Command cmd)
{
    static int initialized = 0;

    // Initialize signals ONLY ONCE
    if (!initialized)
    {
        signal(SIGCHLD, sigchld_handler); // reap background jobs
        signal(SIGINT, SIG_IGN);          // shell ignores Ctrl+C
        initialized = 1;
    }

    if (cmd.command == NULL)
        return;

    // Built-ins
    if (strcmp(cmd.command, "exit") == 0)
        exit(0);

    if (strcmp(cmd.command, "cd") == 0)
    {
        if (cmd.args[1])
            chdir(cmd.args[1]);
        return;
    }

    // External Command
    pid_t pid = fork();

    if (pid == 0)
    {
        // CHILD PROCESS

        // Foreground should respond to Ctrl+C
        if (!cmd.background)
        {
            signal(SIGINT, SIG_DFL);
        }
        // Redirection for open, dup2 must happen here
        if (cmd.background)
        {
            setsid();
        }

        // Input Redirection
        if (cmd.input_file)
        {
            int fd = open(cmd.input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("open input file");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Output Redirection
        if (cmd.output_file)
        {
            int flags = O_WRONLY | O_CREAT;
            flags |= cmd.append ? O_APPEND : O_TRUNC;
            int fd = open(cmd.output_file, flags, 0644);
            if (fd < 0)
            {
                perror("open output file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Execute command
        execvp(cmd.command, cmd.args);
        perror("mysh");
        exit(1);
    }
    else if (pid > 0)
    {
        // PARENT PROCESS

        // WEEK2 - no file redirection needed here (parent waits for the process)
        if (!cmd.background)
        {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0)
                {
                    printf("Command exited with code %d\n", exit_code);
                }
            }
            else if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);
                printf("\n[Process %d killed by signal %d]\n", pid, sig);
                fflush(stdout);
            }
        }
        else
        {
            // WEEK3 - put foreground job where parent does not wait
            static int job_id = 1;
            char cmd_str[1024] = "";

            for (int i = 0; cmd.args[i] != NULL; i++)
            {
                strcat(cmd_str, cmd.args[i]);
                if (cmd.args[i + 1] != NULL)
                {
                    strcat(cmd_str, " ");
                }
            }
            printf("[%d] Started background job: %s (PID: %d)\n", job_id++, cmd_str, pid);
        }
    }
    else
    {
        // fork failure handling
        perror("fork failed");
    }
}