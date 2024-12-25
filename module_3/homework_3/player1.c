#include "chess_clock.h"

int main() {
    int shm_id;
    ChessClock *clock;
    char pathname[] = "w";

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

    printf("shmid: %d\n", shm_id);


    clock = (ChessClock *)shmat(shm_id, NULL, 0);
    if (clock == (void *)-1) {
        perror("Ошибка подключения разделяемой памяти");
        exit(-1);
    }

    if (clock->current_turn == 0 && clock->last_turn_time == 0) {
    // Инициализация разделяемой памяти при первом запуске
    clock->white_time = 0;
    clock->black_time = 0;
    clock->last_turn_time = time(NULL);
    clock->current_turn = 0; // Ход белых
}


    time_t now = time(NULL);
    if (clock->current_turn != 0) {
        printf("Сейчас не ваш ход (ход черных).\n");
        shmdt(clock);
        shmctl(shm_id, IPC_RMID, NULL);
        exit(0);
    }



    // Обновляем время хода
    clock->white_time += now - clock->last_turn_time;
    clock->last_turn_time = now;
    clock->current_turn = 1; // Передаем ход черным

    print_time(clock);
    check_time_limit(clock);

    shmdt(clock);
    return 0;
}
