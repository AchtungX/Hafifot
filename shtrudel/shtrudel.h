#ifndef SHTRUDEL_H
#define SHTRUDEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

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
execv_w_shtrudel - This function execv the commands given in argv, replacing the "@" in the pipe funcuality
            input - (int) shtrudel_idx - the idx of "@" in argv[]
                    (char *[]) argv - the list of argument recived in the program running line
            output -  (int) 0 in case of sucess, -1 otherwise.
*/
int execv_w_shtrudel(int shtrudel_idx, char *argv[]);
