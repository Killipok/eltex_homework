#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "m2_task_4.h"
int main(){
user.Name = "Nekitosss";
user.Surname = "Barboss";
user.Age = 16;
user.GPA = 89;

comp_user.Name = "Nekitosss";
comp_user.Surname = "Barboss";
comp_user.Age = 16;
comp_user.GPA = 89;


printf("Name - %s\nSurname - %s\nAge - %d\nGPA - %d\n", user.Name,user.Surname,user.Age,user.GPA);
printf("Name - %s\nSurname - %s\nAge - %d\nGPA - %d\n", comp_user.Name,comp_user.Surname,comp_user.Age,comp_user.GPA);
printf("Size of user - %ld\n", sizeof(user));
printf("Size of comp_user - %ld\n", sizeof(comp_user));
}

// Вывод pahole:


// loritash@LAPTOP-BLA029HJ:~/eltex/eltex_homework/module_2_task_4$ gcc -g m2_task_4.c m2_task_4.h -o test && pahole test

// struct Person {
//         char *                     Name;                 /*     0     8 */
//         char *                     Surname;              /*     8     8 */
//         int                        Age;                  /*    16     4 */
//         int                        GPA;                  /*    20     4 */

//         /* size: 24, cachelines: 1, members: 4 */
//         /* last cacheline: 24 bytes */
// };
// struct CompressedPerson {
//         char *                     Name;                 /*     0     8 */
//         char *                     Surname;              /*     8     8 */
//         uint8_t                    Age;                  /*    16     1 */
//         uint16_t                   GPA;                  /*    17     2 */

//         /* size: 19, cachelines: 1, members: 4 */
//         /* last cacheline: 19 bytes */
// } __attribute__((__packed__));











