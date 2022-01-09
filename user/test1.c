#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//这个测例模拟了四个并行运行CPU繁忙的进程
int main(){
    int pid0 = fork();
    printf("0\n");
    int pid1 = fork();
<<<<<<< HEAD
    printf("1\n");
    int pid2 = fork();
    printf("2\n");
    if(pid0 == 0){
        sleep(10);
        // printf("I'm pid0\n");
        while(1);
    }
    else if(pid1 == 0){
        sleep(20);
        // printf("I'm pid1\n");
        while(1);
    }
    else if (pid2 == 0){
        sleep(30);
        // printf("I'm pid2\n");
        while(1);
    }
    else{
        sleep(40);
        // printf("I'm the father\n");
        wait(0);
    }
    // for(int i=0;;i++){

    // }

=======
    if(pid0 != 0 && pid1 != 0){ //proc1
      int j = 0;
      for(int i=0; j< 10000000;i++){
          if(i%5 == 0){
              j++;
          }
      }
    }
    else if(pid0 == 0 && pid1 != 0){ //proc2
      int j = 0;
      for(int i=0;j<100000000;i++){
          if(i%15 == 0){
              j++;
          }
      }
    }
    else if(pid0 !=0 && pid1 == 0){ //proc3
      int j = 0;
      for(int i=0;j<100000000;i++){
          if(i%5 == 0){
              j++;
          }
      }
    }
    else{  //proc4
      int j =0;
      for(int i=0;j<10000000;i++){
          if(i%20 == 0){
              j++;
          }
      }
    }
>>>>>>> a527a4806480bd5fde0b0745a125f137feb45bd6
    exit(0);
}