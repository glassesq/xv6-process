#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//这个测例模拟了优先级调度中短作业优先的情况
//首先生成4个并行运行的长作业进程
//在运行的过程中生成10个短作业进程
//可以观察到在优先级调度下短作业优先被处理
int main(){
    int pid0 = fork();
    printf("0\n");
    int pid1 = fork();
    int pid2 = -1;
    if(pid0 != 0 && pid1 != 0){ //proc1
      int j = 0;
      for(int i=0; j< 100000000;i++){
          if(i%10 == 0){
              j++;
          }
      }
    }
    else if(pid0 == 0 && pid1 != 0){ //proc2
      int j = 0;
      for(int i=0;j<100000000;i++){
          if(i%10 == 0){
              j++;
          }
      }
    }
    else if(pid0 !=0 && pid1 == 0){ //proc3
      int j = 0;
      for(int i=0;j<100000000;i++){
        if(i%10 == 0){
          j++;
        }
        if(j%10000000 == 0 && i%10==0){
          pid2 = fork();
        }
        if(pid2 == 0){
          break;
        }
      }
      if(pid2 == 0){
        int k=0;
        for(int i=0;k<10000000;i++){
            if(i%13 == 0){
                k++;
            }
        }
      }
    }
    else{  //proc4
      int j =0;
      for(int i=0;j<100000000;i++){
          if(i%10 == 0){
              j++;
          }
      }
    }
    exit(0);
}