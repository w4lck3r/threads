#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>

/**
 * we will make a function that will keep track of the number of threads that
 * are currently running, we could start off by having global_i as a global counter = N (N=nb threads)
 * where global_i is that number of threads that haven't finished executing a yet
 * evertime we finish executing a we will lower the global counter by 1, and then
 * test to see if there are any threads that haven't finished a yet
 * the test consists of checking if that global number is equala to 0, if not keep waiting
 * if it's equal to zero then call the post function so all the threads can start executing b
 **/
#define N 101 // comme demandee a l'enonce du tp
sem_t sem1, sem2;
int global_i = N;

void a(int i) {
    // sleep(1);
    printf("a%d\n", i);
    // sleep(1);
}

void b(int i) {
    // sleep(1);
    printf("b%d\n", i);
    // sleep(1);
}

void* checkCounter(){
    global_i -= 1;
    if (global_i == 0){
        for(int i = 0; i < N; i++){
            assert(sem_post(&sem1) == 0);
        }
    }

}

void *p(int i) {
    a(i);
    checkCounter();
    assert(sem_wait(&sem1) == 0);
    b(i);
    return NULL;
}

int main(int argc, char* argv[]) {
    assert(argc > 1);

    sem_t sem[N];
    pthread_t pth[N];

    assert(sem_init(&sem1, 0, 0) == 0);
    assert(sem_init(&sem2, 0, 0) == 0);

    for(int i = 0; i < N; i++){
        pthread_create(&pth[i], NULL, &p, i);
    }

    for(int i = 0; i < N; i++){
        pthread_join(pth[i], NULL);
    }

    return 0;
}


