#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global_var = 10;

int main() {
    int local_var = 20;
    int pid;

    printf("Global: %p, %d\n", &global_var, global_var);
    printf("Local: %p, %d\n", &local_var, local_var);

    printf("Parent PID: %d\n", getpid());

    pid = fork();

    if (pid == 0) {
        printf("Children\nPID: %d\n", getpid());
        printf("Parent PID : %d\n", getppid());
        sleep(10);

        printf("Global: %p, %d\n", &global_var, global_var);
        printf("Local: %p, %d\n", &local_var, local_var);

        global_var = 30;
        local_var = 40;

        printf("Changes: %d, %d\n", global_var, local_var);
        printf("Global: %p, %d\n", &global_var, global_var);
        printf("Local: %p, %d\n", &local_var, local_var);

        exit(5);
    } else if (pid > 0) {

        sleep(30);

        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Exit code: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Signal num: %d\n", WTERMSIG(status));
        }

        printf("Parent\nGlobal: %p, %d\n", &global_var, global_var);
        printf("Local: %p, %d\n", &local_var, local_var);
    } else {
        printf("Error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
