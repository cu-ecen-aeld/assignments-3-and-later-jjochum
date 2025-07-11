#include "systemcalls.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
* @param cmd the command to execute with system()
* @return true if the command in @param cmd was executed
*   successfully using the system() call, false if an error occurred,
*   either in invocation of the system() call, or if a non-zero return
*   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd) {
    
    int rc = system(cmd);
    return (rc == 0);
}

/**
* @param count -The numbers of variables passed to the function. The variables
* are command to execute. followed by arguments to pass to the command Since
* exec() does not perform path expansion, the command to execute needs to be an
* absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in
* execv()
* @return true if the command @param ... with arguments @param arguments were
* executed successfully using the execv() call, false if an error occurred,
* either in invocation of the fork, waitpid, or execv() command, or if a
* non-zero return value was returned by the command issued in @param arguments
* with the specified arguments.
*/

bool do_exec(int count, ...) {
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    bool ret = true;
    
    pid_t pid = fork();
    if (pid == 0) {
        setsid();
        close(0);
        close(1);
        close(2);
        execv(command[0], command);
        exit(EXIT_FAILURE);
    } else if (pid == -1) {
        ret = false;
    } else {
        int status = 0;
        int rc = waitpid(-1, &status, 0);
        if (rc == -1)
        {
            ret = false;
        }
        else
        {
            int exited_normally = WIFEXITED(status);
            int exit_status = WEXITSTATUS(status);
            if (exited_normally == 0)
            {
                if (exit_status == 0)
                {
                    return true;
                } else {
                    return false;
                }
            } else {
                if (exit_status == 0)
                {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    va_end(args);
    return ret;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...) {
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    bool ret = true;
    
    int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) {
        return false;
    }
    pid_t pid = fork();
    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);
        close(fd);      
        execv(command[0], command);
        exit(EXIT_FAILURE);
    } else if (pid == -1) {
        return false;
    } else {
        int status;
        close(fd);
        if (waitpid(-1, &status, 0) == -1)
        {
            ret = false;
        }
        else {
            int exited_normally = WIFEXITED(status);
            int exit_status = WEXITSTATUS(status);
            if (exited_normally == 0)
            {
                if (exit_status == 0)
                {
                    return true;
                } else {
                    return false;
                }
            } else {
                if (exit_status == 0)
                {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    va_end(args);
    return ret;
}
