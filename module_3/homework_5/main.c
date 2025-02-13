/*
gcc -pthread -o signal_threads main.c
./signal_threads 
Получаем PID командой pgrep signal_threads
kill -SIGINT pid
kill -SIGQUIT pid
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg) {
    int num = *(int *)arg;
    printf("Thread %d started\n", num);
    while (1) {
        sleep(1);
    }
    return NULL;
}

void sigint_handler(int signum) {
    printf("Received SIGINT in handler\n");
}

void *sigquit_thread(void *arg) {
    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGQUIT);
    if (sigwait(&sigset, &sig) == 0) {
        printf("Received SIGQUIT via sigwait\n");
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};
    sigset_t set;

    // Блокируем все сигналы
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // Устанавливаем обработчик SIGINT
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // Создаём потоки
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }
    pthread_create(&threads[2], NULL, sigquit_thread, &ids[2]);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
