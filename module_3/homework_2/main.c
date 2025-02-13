#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("PID: %d\n", getpid());
        sleep(1);
        execl("/proc/self/exe", "/proc/self/exe", "hello world", NULL);
        perror("exec failed");
        return 1;
    } else {
        printf("%s\n", argv[1]);
        return 0;
    }
}
