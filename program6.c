#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

struct shared_data {
    int multiple;
    int counter;
};

int main() {
    key_t key = ftok("sharedfile", 65); // Generate unique key
    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    struct shared_data *data = (struct shared_data *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    data->multiple = 3;    
    data->counter = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        execl("./program7", "program7", (char *)NULL);
        perror("execl failed");
        exit(1);
    } 
    else {
        while (data->counter <= 500) {
            if (data->counter % data->multiple == 0) {
                printf("Process 1 (PID %d): counter=%d is a multiple of %d\n",
                       getpid(), data->counter, data->multiple);
            } else {
                printf("Process 1 (PID %d): counter=%d\n", getpid(), data->counter);
            }

            data->counter++;
            sleep(1);
        }

        printf("Process 1 (PID %d): counter exceeded 500. Ending.\n", getpid());

        wait(NULL);

        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);

        printf("Shared memory released. Exiting Process 1.\n");
    }

    return 0;
}
