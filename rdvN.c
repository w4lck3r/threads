#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>

int Ng = 0;

void a(int i) {
    sleep(1);
    printf("a%d\n", i);
    sleep(1);
}

void b(int i) {
    sleep(1);
    printf("b%d\n", i);
    sleep(1);
}

struct p_arg_s{
    int i;
    sem_t sem;

};

void *p(void *arg) {
    struct p_arg_s* structure = (struct p_arg_s*) arg;
    int i = structure->i;
    a(i);
    for (int j = 0; j < Ng; j++){
        if(j != i){
            assert(sem_post(&structure->sem) == 0);
        }
    }
    for (int j = 0; j < Ng - 1; j++){
        assert(sem_wait(&structure->sem) == 0);
    }
    b(i);
    return NULL;
}

int main(int argc, char* argv[]) {
    assert(argc > 1);
    int N = atoi(argv[1]);
    Ng = N;

    sem_t sem[N];
    pthread_t pth[N];
    struct p_arg_s args[N];

    for(int i = 0; i < N; i++){
        assert(sem_init(&sem[i], 0, 0) == 0);
        args[i].i = i;
        args[i].sem = sem[i];
    }

    for(int i = 0; i < N; i++){
        pthread_create(&pth[i], NULL, &p, &args[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(pth[i], NULL);
    }

    return 0;
}


