#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CPUPROC 4
#define IOPROC  0
#define CROSS
// 创建IO和CPU进程

int main(){
    int pid;
    printf("here\n");
    for(int i = 0; i < CPUPROC + IOPROC; i++){
        printf("%d\n", i);
        pid = fork();
        if(pid < 0){
            break;      // Error
        }
        if(pid == 0){
            #ifndef CROSS
            if(i < CPUPROC){    // 这些子进程为CPU繁忙
            #else
            if(i % 2){
            #endif
                int temp = 0;
                while(temp < 1000000000ll){
                    temp++;
                }
                break;
            }
            else{               // 这些子进程为IO繁忙
                for(int i=0;i<100000000;i++){
                    char buf[100];
                    int k = 0;
                    if(i%60000000==0){
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
                break;
            }
        }
        else{    
            // wait(0);           
            continue;
        }
    }
    while(wait(0)!=-1){}
    exit(0);
}