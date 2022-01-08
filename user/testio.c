#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    for(int i=0;i<100000000;i++){
        char buf[100];
        int k = 0;
        if(i%25000000==0){
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
    exit(0);
}