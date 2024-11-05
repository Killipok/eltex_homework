#include <stdio.h>
#define TWO 2

int main(){
    int N = 0;
    int result = 1;
    printf("Введите натуральное число\n");
    scanf("%d", &N);
    for(int i = 0; i < N; i++){
        result = TWO * result;
    }
    
    printf("%d", result);

}