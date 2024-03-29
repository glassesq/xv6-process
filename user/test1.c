#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//这个测例模拟了四个并行运行CPU繁忙的进程
int main(){
    int pid0 = fork();
    printf("0\n");
    int pid1 = fork();
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
    exit(0);
}