#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CPUPROC 10
#define IOPROC  10
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
            if(i < CPUPROC){    // 这些子进程为CPU繁忙
                int temp = 0;
                while(temp < 1000000000ll){
                    temp++;
                }
                break;
            }
            else{               // 这些子进程为IO繁忙
                char buf[100];
                int count = 0;
                while(count < 1000){
                    memset(buf,0,sizeof(buf));
                    // gets(buf,sizeof(buf));
                    if(buf[0] != 0){
                        fprintf(2,buf);
                    }
                    count++;
                }
                break;
            }
        }
        else{               
            continue;
        }
    }
    exit(0);
}