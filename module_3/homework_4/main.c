/*
gcc -o chess main.c -lrt -lpthread
./chess
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_MOVES 100
#define MOVE_LEN 20

typedef struct {
    char moves[MAX_MOVES][MOVE_LEN];
    int count;
} SharedData;

void player(const char *sem_name, const char *other_sem_name,
           const char *moves[], int num_moves, SharedData *shared,
           const char *player_name) {
    sem_t *sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem_t *other_sem = sem_open(other_sem_name, 0);
    if (other_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_moves; i++) {
        sem_wait(sem);
        strncpy(shared->moves[shared->count], moves[i], MOVE_LEN-1);
        shared->moves[shared->count][MOVE_LEN-1] = '\0';
        shared->count++;

        printf("%s делает ход: %s\n", player_name, moves[i]);
        fflush(stdout);

        sem_post(other_sem);
    }

    sem_close(sem);
    sem_close(other_sem);
}

int main() {
    const char *moves_white[] = {"c4", "Nf3", "d4", "Nc3", "Bg5",
        "e3", "Bh4", "cxd5", "Bxe7", "Nxd5",
        "Rc1", "Qa4", "Qa3", "Bb5", "dxc5",
        "O-O", "Be2", "Nd4", "Nxe6", "e4",
        "f4", "e5", "Bc4", "Qh3", "b3",
        "f5", "Rxf5", "Rcf1", "Qg3", "h4",
        "e6", "Qe5", "a4", "R1f2", "R2f3",
        "Bd3", "Qe4", "Rxf6", "Rxf6", "Bc4",
        "Qf4"};

    const char *moves_black[] = {"e6", "d5", "Nf6", "Be7", "O-O",
        "h6", "b6", "Nxd5", "Qxe7", "exd5",
        "Be6", "c5", "Rc8", "a6", "bxc5",
        "Ra7", "Nd7", "Qf8", "fxe6", "d4",
        "Qe7", "Rb8", "Kh8", "Nf8", "a5",
        "exf5", "Nh7", "Qd8", "Re7", "Rbb7",
        "Rbc7", "Qe8", "Qd8", "Qe8", "Qd8",
        "Qe8", "Nf6", "gxf6", "Kg8", "Kh8"};

    int num_white = sizeof(moves_white)/sizeof(moves_white[0]);
    int num_black = sizeof(moves_black)/sizeof(moves_black[0]);

    sem_unlink("/sem_white");
    sem_unlink("/sem_black");

    int fd = shm_open("/chess_shm", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    SharedData *shared = mmap(NULL, sizeof(SharedData),
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shared->count = 0;

    sem_t *sem_white = sem_open("/sem_white", O_CREAT, 0666, 1);
    sem_t *sem_black = sem_open("/sem_black", O_CREAT, 0666, 0);

    if (sem_white == SEM_FAILED || sem_black == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == 0) {
        player("/sem_white", "/sem_black", moves_white, num_white,
              shared, "Белые");
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            player("/sem_black", "/sem_white", moves_black, num_black,
                  shared, "Черные");
            exit(EXIT_SUCCESS);
        }
        else if (pid2 > 0) {
            waitpid(pid, NULL, 0);
            waitpid(pid2, NULL, 0);

            printf("\nПоследовательность ходов:\n");
            for (int i = 0; i < shared->count; i++) {
                printf("%s ", shared->moves[i]);
            }
            printf("\n");

            munmap(shared, sizeof(SharedData));
            shm_unlink("/chess_shm");
            sem_close(sem_white);
            sem_close(sem_black);
            sem_unlink("/sem_white");
            sem_unlink("/sem_black");
        }
        else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
