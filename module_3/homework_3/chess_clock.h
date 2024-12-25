#ifndef CHESS_CLOCK_H
#define CHESS_CLOCK_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHM_SIZE sizeof(ChessClock)

#define TIME_LIMIT 60

typedef struct {
    time_t white_time; // Время белых
    time_t black_time; // Время черных
    time_t last_turn_time; // Время последнего хода
    int current_turn; // 0 — белые, 1 — черные
} ChessClock;

void print_time(ChessClock *clock) {
    printf("Белые: %ld сек, Черные: %ld сек\n", clock->white_time, clock->black_time);
}

void check_time_limit(ChessClock *clock) {
    if (clock->white_time > TIME_LIMIT) {
        printf("Техническое поражение белых.\n");
        system("bash ipcrm.bash");
        exit(0);
    } else if (clock->black_time > TIME_LIMIT) {
        printf("Техническое поражение черных.\n");
        system("bash ipcrm.bash");
        exit(0);
    }
}

#endif // CHESS_CLOCK_H
