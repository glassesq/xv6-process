#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
	printf("Hello World!\n");
    int i = 1;
    while(i%100000){
        i++;
    }
	exit(0);
}