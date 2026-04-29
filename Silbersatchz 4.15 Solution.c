#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MIN_PID 300
#define MAX_PID 5000
#define RANGE (MAX_PID - MIN_PID + 1)
#define THREADS 100

int pid_map[RANGE];
pthread_mutex_t lock;

int allocate_map() {
    for (int i = 0; i < RANGE; i++)
        pid_map[i] = 0;
    return 1;
}

int allocate_pid() {
    pthread_mutex_lock(&lock);

    for (int i = 0; i < RANGE; i++) {
        if (pid_map[i] == 0) {
            pid_map[i] = 1;
            pthread_mutex_unlock(&lock);
            return i + MIN_PID;
        }
    }

    pthread_mutex_unlock(&lock);
    return -1;
}

void release_pid(int pid) {
    pthread_mutex_lock(&lock);
    pid_map[pid - MIN_PID] = 0;
    pthread_mutex_unlock(&lock);
}

void* worker(void* arg) {
    int pid = allocate_pid();

    if (pid == -1) {
        printf("No PID available\n");
        return NULL;
    }

    printf("Thread %ld got PID %d\n", pthread_self(), pid);

    sleep(rand() % 5 + 1);

    release_pid(pid);
    printf("Thread %ld released PID %d\n", pthread_self(), pid);

    return NULL;
}

int main() {
    pthread_t tid[THREADS];

    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);

    allocate_map();

    for (int i = 0; i < THREADS; i++)
        pthread_create(&tid[i], NULL, worker, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
