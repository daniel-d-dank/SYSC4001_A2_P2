#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

struct shared_data {
    int multiple;
    int counter;
};

// Semaphore operations
struct sembuf sem_lock = {0, -1, 0};  
struct sembuf sem_unlock = {0, 1, 0}; 

int main() {
    key_t key = ftok("sharedfile", 65);

    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid < 0) { perror("shmget"); exit(1); }

    struct shared_data *data = (struct shared_data *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) { perror("shmat"); exit(1); }

    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid < 0) { perror("semget"); exit(1); }

    semctl(semid, 0, SETVAL, 1);

    // Initialize shared memory values
    data->multiple = 3;
    data->counter = 0;

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }
    else if (pid == 0) {
        execl("./program9", "program9", (char *)NULL);
        perror("execl");
        exit(1);
    } 
    else {
        while (1) {
            semop(semid, &sem_lock, 1);

            if (data->counter > 500) {
                semop(semid, &sem_unlock, 1);
                break;
            }

            if (data->counter % data->multiple == 0) {
                printf("Process 1 (PID %d): counter=%d is a multiple of %d\n",
                       getpid(), data->counter, data->multiple);
            } else {
                printf("Process 1 (PID %d): counter=%d\n", getpid(), data->counter);
            }

            data->counter++;

            semop(semid, &sem_unlock, 1);

            sleep(1);
        }

        printf("Process 1: counter exceeded 500. Waiting for child...\n");
        wait(NULL);

        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);  

        printf("Cleaned up shared memory and semaphore. Exiting Process 1.\n");
    }

    return 0;
}
