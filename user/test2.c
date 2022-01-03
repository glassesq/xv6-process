#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    char buf[100];
    fork();
    fork();
    while(1){
        memset(buf,0,sizeof(buf));
        gets(buf,sizeof(buf));
        if(buf[0] != 0){
            fprintf(2,buf);
        }
    }
    exit(0);
}
