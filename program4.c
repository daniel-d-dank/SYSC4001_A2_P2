#include <stdio.h>
#include <unistd.h>     
#include <stdlib.h>     
#include <sys/types.h>  
#include <sys/wait.h>  

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        execl("./program5", "program5", (char *)NULL);
        perror("Exec failed");
        exit(1);
    }
    else {
        int counter = 0;
        int cycle = 0;

        printf("Process 1 (PID %d): waiting for Process 2 (PID %d) to start...\n", getpid(), pid);
        sleep(1);

        while (1) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);

            if (result == 0) {
                if (counter % 3 == 0) {
                    printf("Process 1 (PID %d) - Cycle %d: %d is a multiple of 3\n", getpid(), cycle, counter);
                } else {
                    printf("Process 1 (PID %d) - Cycle %d\n", getpid(), cycle);
                }

                counter++;
                cycle++;
                sleep(1);
            } 
            else {
                printf("Process 2 (PID %d) has finished. Process 1 ending.\n", pid);
                break;
            }
        }
    }

    return 0;
}
