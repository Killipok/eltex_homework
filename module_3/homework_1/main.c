#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "./sig_fifo"

void sigabrt_handler(int signum) {
    (void)signum; 

    int fifo_fd = open(FIFO_PATH, O_WRONLY); 
    if (fifo_fd == -1) {
        perror("open FIFO");
        exit(EXIT_FAILURE);
    }

    char msg[50];
    snprintf(msg, sizeof(msg), "Received SIGABRT (PID: %d)\n", getpid());
    write(fifo_fd, msg, strlen(msg));
    close(fifo_fd);

    printf("Записано сообщение в FIFO и завершение программы.\n");
    exit(EXIT_FAILURE);
}

int main() {
    unlink(FIFO_PATH);

    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
    }

    struct sigaction sa;
    sa.sa_handler = sigabrt_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGABRT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d. Send SIGABRT to test.\n", getpid());

    while (1) {
        pause(); 
    }

    return 0;
}
