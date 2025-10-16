#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct shared_data {
    int multiple;
    int counter;
};

struct sembuf sem_lock = {0, -1, 0};  
struct sembuf sem_unlock = {0, 1, 0}; 

int main() {
    key_t key = ftok("sharedfile", 65);

    int shmid = shmget(key, sizeof(struct shared_data), 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }

    int semid = semget(key, 1, 0666);
    if (semid < 0) { perror("semget"); exit(1); }

    struct shared_data *data = (struct shared_data *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) { perror("shmat"); exit(1); }

    printf("Process 2 (PID %d): waiting for counter > 100...\n", getpid());

    while (1) {
        semop(semid, &sem_lock, 1);

        if (data->counter > 500) {
            semop(semid, &sem_unlock, 1);
            break;
        }

        if (data->counter <= 100) {
            semop(semid, &sem_unlock, 1);
            sleep(1);
            continue;
        }

        if (data->counter % data->multiple == 0) {
            printf("Process 2 (PID %d): counter=%d is a multiple of %d\n",
                   getpid(), data->counter, data->multiple);
        } else {
            printf("Process 2 (PID %d): counter=%d\n", getpid(), data->counter);
        }

        semop(semid, &sem_unlock, 1);
        sleep(1);
    }

    printf("Process 2 (PID %d): counter exceeded 500. Exiting.\n", getpid());

    shmdt(data);
    return 0;
}