#ifndef SHTRUDEL_H
#define SHTRUDEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define first_command_idx 1

#define CHECK(val, log_msg) \
    do { \
        if (!(val)) { \
            printf("%s\n", log_msg); \
            goto error; \
        } \
    } while (0)

#endif

/*
get_shtrudel_idx - This function get the "@" idx inside argv[]
            input - (int) argc - the number of elements in the program running line
                    (char *[]) argv - the list of argument recived in the program running line
            output -  (int) the idx of "@" in argv[], zero in case no "@" was found
*/
int get_shtrudel_idx(int argc, char *argv[]);

/*
check_shtrudel_and_execv - This function checks if there is another "@" in argv, if there isnt it run the sh command. if there is it calls the execv_w_shtrudel
                            this function being called in recursion. 
            input - (int) before_shtrudel_command_idx - the idx of first command to run, the one before the "@" if there is a "@". 
                    (int) argc - the number of elements in the program running line
                    (char *[]) argv - the list of argument recived in the program running line
            output -  (int) 0 in case of sucess, -1 otherwise.
*/
int check_shtrudel_and_execv(int before_shtrudel_command_idx, int argc, char *argv[]);

/*
execv_w_shtrudel - This function execv the commands given in argv, replacing the "@" in the pipe funcuality, 
                    the before shtrudel command run in the father process and the after shtrudel command that run in the child proccess 
                    looks for another "@" in the argv using get_shtrudel_idx and calls check_shtrudel_and_execv again.
            input - (int) before_shtrudel_command_idx - the idx of first command to run, the one before the "@" if there is a "@". 
                    (int) shtrudel_idx - the idx of "@" in argv[]
                    (int) argc - the number of elements in the program running line
                    (char *[]) argv - the list of argument recived in the program running line
            output -  (int) 0 in case of sucess, -1 otherwise.
*/
int execv_w_shtrudel(int before_shtrudel_command_idx, int shtrudel_idx, int argc, char *argv[]);
