#include <stdio.h>
#include <unistd.h>    
#include <sys/types.h>  

int main() {
    pid_t pid = fork();  

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } 
    else if (pid == 0) {
        int counter = 0;
        while (1) {
            printf("Child process (PID %d): counter = %d\n", getpid(), counter++);
            sleep(1); 
        }
    } 
    else {
        int counter = 0;
        while (1) {
            printf("Parent process (PID %d): counter = %d\n", getpid(), counter++);
            sleep(1); 
        }
    }

        return 0;
}