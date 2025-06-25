#include "shtrudel.h"

int main(int argc, char *argv[]) {
    CHECK(argc >= 2, "missing sh command");

    int shtrudel_idx = get_shtrudel_idx(argc, argv);

    if(shtrudel_idx == 0) { //No shtrudel recived , exec one sh command
        execv(argv[1], &argv[1]);
        CHECK(false, "Failed to exec"); //If this line runs it means execv failed
    }
    else {
        argv[shtrudel_idx] = NULL; //Set "@" to NULL - to seperate between the commands"
        int ret = execv_w_shtrudel(shtrudel_idx, argv); //Preform two command with pipe
        CHECK(ret ==0, "Failed to exec with shtrudel");
    }

    return 0;
error:
    return -1;
}

int get_shtrudel_idx(int argc, char *argv[]) {
    for (int i =0; i < argc; i++) {
        if(strncmp(argv[i], "@", strlen(argv[i])) == 0) { //Get shtrudel idx
            return i;
        }
    }
    return 0; //Zero is the error return value beacuse argv[0] is this function exec path and cant be @.
}

int execv_w_shtrudel(int shtrudel_idx, char *argv[]){
    int pipefd[2];
    CHECK(pipe(pipefd) == 0, "Failed to do pipe");
    
    __pid_t pid;
    pid = fork();
    CHECK(pid >=0, "Failed to do fork");

    if(pid ==0) { //In child process, do second command, after shtrudel
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO); //Set input fd to the pipe input
        execv(argv[shtrudel_idx+1], &argv[shtrudel_idx+1]);
        CHECK(false, "Failed to exec second command"); //If this line runs it means execv failed
    }
    else { //In parent process, do first command, before shtrudel
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); //Set output fd to the pipe output
        execv(argv[1], &argv[1]);
        CHECK(false, "Failed to exec first command"); //If this line runs it means execv failed
    }

    return 0;
error:
    return -1; 
}
