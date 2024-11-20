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
printf("Size of user - %zu\n", sizeof(user));
printf("Size of comp_user - %zu\n", sizeof(comp_user));
}
