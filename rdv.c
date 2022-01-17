#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem1, sem2;

void a(int i) {
    // sleep(1);
    printf("a = %d\n", i);
    // sleep(1);
}

void b(int i) {
    // sleep(1);
    printf("b = %d\n", i);
    // sleep(1);
}

void *p1(void *arg) {
    assert(arg == NULL);
    a(1);
    assert(sem_post(&sem2) == 0);
    assert(sem_wait(&sem1) == 0);
    b(1);
    return NULL;
}

void *p2(void *arg) {
    assert(arg == NULL);
    a(2);
    assert(sem_post(&sem1) == 0);
    assert(sem_wait(&sem2) == 0);
    b(2);
    return NULL;
}

int main() {
    assert(sem_init(&sem1, 0, 0) == 0);
    assert(sem_init(&sem2, 0, 0) == 0);

    pthread_t pth1, pth2;
    pthread_create(&pth1, NULL, &p1, NULL );
    pthread_create(&pth2, NULL, &p2, NULL );
    pthread_join(pth1, NULL);
    pthread_join(pth2, NULL);

    return 0;
}


