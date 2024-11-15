#include <stdio.h>
#define TWO 2

int main(){
    int N = 0;
    int result = 1;
    int *pN = &N;
    int *Presult = &result;
    printf("Введите натуральное число\n");
    scanf("%d", pN);
    for(int i = 0; i < *pN; i++){
        *Presult = TWO * (*Presult);
    }

    printf("%d", *Presult);
}