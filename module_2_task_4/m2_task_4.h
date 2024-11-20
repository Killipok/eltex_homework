#ifndef M2_TASK_4_H_
#define M2_TASK_4_H_
#include <stdalign.h>
#include <stdint.h>
struct Person
{
    char* Name;
    char* Surname;
    int Age;
    int GPA;
} user;

/*
У меня не получилось с alignas но в интернете нашел "__attribute__((packed))"
 он убирает автоматически добавленные компилятором байты между полями структуры
 Компилировал командой: gcc -g m2_task_4.c m2_task_4.h -o test && pahole test
*/
struct  __attribute__((packed)) CompressedPerson 
{
     char* Name;
     char* Surname;
     uint8_t Age;
     uint16_t GPA;
}comp_user;




#endif //M2_TASK_4_H_