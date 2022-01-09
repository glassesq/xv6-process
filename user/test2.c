#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//这个测例模拟了四个并行运行的进程
//其中proc3为IO繁忙的进程
int main(){
    int pid0 = fork();
    int pid1 = fork();
    if(pid0 != 0 && pid1 != 0){ //proc1
      for(int i=0;i<100000000;i++){
        char buf[100];
        int k = 0;
        if(i%5000000==0){
            while(k<1){
                memset(buf,0,sizeof(buf));
                gets(buf,sizeof(buf));
                if(buf[0] != 0){
                    fprintf(2,buf);
                    k++;  
                }  
            }
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
      for(int i=0; j< 10000000;i++){
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