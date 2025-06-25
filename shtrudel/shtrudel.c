#include "shtrudel.h"

int main(int argc, char *argv[]) {
    CHECK(argc >= 2, "missing sh command");

    int ret = check_shtrudel_and_execv(first_command_idx, argc, argv);
    CHECK(ret ==0, "Failed in check_shtrudel_and_execv func");

    return 0;
error:
    return -1;
}

int get_shtrudel_idx(int argc, char *argv[]) {
    for (int i =0; i < argc; i++) {
        if(argv[i] == NULL) { //Skip the already found "@" that have been turn into NULL
            continue;
        }
        if(strncmp(argv[i], "@", strlen(argv[i])) == 0) { //Get shtrudel idx
            argv[i] = NULL; //Set "@" to NULL - to seperate between the commands after in execv"
            return i;
        }
    }
    return 0; //Zero is the error return value beacuse argv[0] is this function exec path and cant be @.
}

int check_shtrudel_and_execv(int before_shtrudel_command_idx, int argc, char *argv[]) {
    int shtrudel_idx = get_shtrudel_idx(argc, argv); //Get shturdel_idx

    if(shtrudel_idx == 0) { //No shtrudel left or recived , exec sh command
        execv(argv[before_shtrudel_command_idx], &argv[before_shtrudel_command_idx]);
        CHECK(false, "Failed to exec"); //If this line runs it means execv failed
    }
    else {
        int ret = execv_w_shtrudel(before_shtrudel_command_idx, shtrudel_idx, argc, argv); //Preform two command with pipe
        CHECK(ret ==0, "Failed to exec with shtrudel");
    }

    return 0;
error:
    return -1;
}

int execv_w_shtrudel(int before_shtrudel_command_idx, int shtrudel_idx, int argc, char *argv[]) {
    int pipefd[2];
    CHECK(pipe(pipefd) == 0, "Failed to do pipe");
    
    __pid_t pid;
    pid = fork();
    CHECK(pid >=0, "Failed to do fork");

    if(pid ==0) { //In child process, do second command, after shtrudel
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO); //Set input fd to the pipe input
        int ret = check_shtrudel_and_execv(shtrudel_idx+1, argc, argv); //Run the program again - recursion
        CHECK(ret == 0, "Failed in check_shtrudel_and_execv func");
    }
    else { //In parent process, do first command, before shtrudel
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); //Set output fd to the pipe output
        execv(argv[before_shtrudel_command_idx], &argv[before_shtrudel_command_idx]);
        CHECK(false, "Failed to exec command"); //If this line runs it means execv failed
    }

    return 0;
error:
    return -1; 
}
