#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int pid0 = fork();
    printf("0\n");
    int pid1 = fork();
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

    exit(0);
}