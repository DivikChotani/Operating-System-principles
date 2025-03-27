#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EINVAL;
    }

    int commands = argc - 1;
    int *procs = malloc(commands * sizeof(int));
    if (procs == NULL) {
        return errno;
    }
    
    int (*pipes)[2] = malloc((commands - 1) * sizeof(*pipes));
    if (pipes == NULL) {
        free(procs);
        return errno;
    }
    
    for (int i = 0; i < commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            free(pipes);
            free(procs);
            return errno;
        }
    }

    for (int i = 0; i < commands; i++) {
        procs[i] = fork();
        if (procs[i] < 0) {
            for (int j = 0; j < i; j++) {
                waitpid(procs[j], NULL, 0);
            }
            for (int j = 0; j < commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            free(pipes);
            free(procs);
            return errno;
        }

        if (procs[i] == 0) { 
            if (i != 0) {
                if (dup2(pipes[i-1][0], STDIN_FILENO) == -1) {
                    exit(errno);
                }
            }
            if (i != commands - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    exit(errno);
                }
            }

            for (int q = 0; q < commands - 1; q++) {
                if (close(pipes[q][0]) == -1) {
                    exit(errno);
                }
                if (close(pipes[q][1]) == -1) {
                    exit(errno);
                }
            }
            
            free(pipes);
            free(procs);

            execlp(argv[i+1], argv[i+1], NULL);
            exit(errno);  
        }
    }

    for (int i = 0; i < commands - 1; i++) {
        if (close(pipes[i][0]) == -1) {
            int save_errno = errno;
            for (int j = 0; j < commands; j++) {
                waitpid(procs[j], NULL, 0);
            }
            free(pipes);
            free(procs);
            return save_errno;
        }
        if (close(pipes[i][1]) == -1) {
            int save_errno = errno;
            for (int j = 0; j < commands; j++) {
                waitpid(procs[j], NULL, 0);
            }
            free(pipes);
            free(procs);
            return save_errno;
        }
    }

    int status;
    for (int i = 0; i < commands; i++) 
    {
        waitpid(procs[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
        {
            for (int j = i + 1; j < commands; j++) 
            {
                waitpid(procs[j], NULL, 0);
            }

            free(pipes);
            free(procs);

            errno = WEXITSTATUS(status);
            return errno;
        }
    }

    
    free(pipes);
    free(procs);
    return 0;

}