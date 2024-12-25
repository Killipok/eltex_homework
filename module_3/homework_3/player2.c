#include "chess_clock.h"

int main() {
    char pathname[] = "w";
    int shm_id;
    int current_turn = 2;

    ChessClock *clock;
    key_t key = ftok(pathname, 0);
    if (key == -1) {
        perror("Ошибка генерации ключа");
        exit(-1);
    }

    shm_id = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("Ошибка создания разделяемой памяти");
        exit(-1);
    }

    clock = (ChessClock *)shmat(shm_id, NULL, 0);
    if (clock == (void *)-1) {
        perror("Ошибка подключения разделяемой памяти");
        exit(-1);
    }

    time_t now = time(NULL);
    if (clock->current_turn != 1) {
        printf("Сейчас не ваш ход (ход белых).\n");
        shmdt(clock);
        exit(0);
    }

    // Обновляем время хода
    clock->black_time += now - clock->last_turn_time;
    clock->last_turn_time = now;
    clock->current_turn = 0; // Передаем ход белым

    print_time(clock);
    check_time_limit(clock);

    shmdt(clock);
    return 0;
}
