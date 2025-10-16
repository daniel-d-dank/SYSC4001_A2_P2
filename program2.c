#include <stdio.h>
#include <unistd.h>     
#include <stdlib.h>     
#include <sys/types.h>  

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // CHILD PROCESS: launch process2 using exec()
        execl("./program3", "program3", (char *)NULL);

        perror("Exec failed");
        exit(1);
    }
    else {
        // PARENT PROCESS 
        int counter = 0;
        int cycle = 0;

        while (1) {
            if (counter % 3 == 0) {
                printf("Process 1 (PID %d) - Cycle %d: %d is a multiple of 3\n", getpid(), cycle, counter);
            } else {
                printf("Process 1 (PID %d) - Cycle %d\n", getpid(), cycle);
            }

            counter++;
            cycle++;
            sleep(1); 
        }
    }

    return 0;
}