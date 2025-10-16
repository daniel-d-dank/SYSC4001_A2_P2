#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

struct shared_data {
    int multiple;
    int counter;
};

int main() {
    key_t key = ftok("sharedfile", 65); 
    int shmid = shmget(key, sizeof(struct shared_data), 0666);
    if (shmid < 0) {
        perror("shmget failed in process2");
        exit(1);
    }

    struct shared_data *data = (struct shared_data *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat failed in process2");
        exit(1);
    }

    printf("Process 2 (PID %d): waiting for counter > 100...\n", getpid());

    while (data->counter <= 100) {
        sleep(1);
    }

    printf("Process 2 (PID %d): started running (multiple=%d)\n", getpid(), data->multiple);

    while (data->counter <= 500) {
        if (data->counter % data->multiple == 0) {
            printf("Process 2 (PID %d): counter=%d is a multiple of %d\n",
                   getpid(), data->counter, data->multiple);
        } else {
            printf("Process 2 (PID %d): counter=%d\n", getpid(), data->counter);
        }
        sleep(1);
    }

    printf("Process 2 (PID %d): counter exceeded 500. Exiting.\n", getpid());

    shmdt(data);

    return 0;
}
