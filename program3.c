#include <stdio.h>
#include <unistd.h>     

int main() {
    int counter = 0;
    int cycle = 0;

    while (1) {
        if (counter % 3 == 0) {
            printf("Process 2 (PID %d) - Cycle %d: %d is a multiple of 3\n", getpid(), cycle, counter);
        } else {
            printf("Process 2 (PID %d) - Cycle %d\n", getpid(), cycle);
        }

        counter--; 
        cycle++;
        sleep(1); 
    }

    return 0;
}