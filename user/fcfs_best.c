#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int pid;
    printf("here\n");
    for(int i = 0; i < 5; i++){
        printf("%d\n", i);
        pid = fork();
        if(pid < 0){
            break;      // Error
        }
        if(pid == 0){
            long long temp = 0;
            while(temp < 1000000000ll * (i + 1)){
                temp++;
            }
            break;
        }
        else continue;
    }
    while(wait(0)!=-1){}
    exit(0);
}