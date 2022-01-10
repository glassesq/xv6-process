#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char** argv) {
  printf("pipetest: say hi\n");
  if(argc != 3) {
    printf("wrong arguments\n");
    exit(0);
  }
  int COUNT = atoi(argv[1]);
  int SIZE = atoi(argv[2]);



  int p[2];
  pipe(p);
  int READ = 0, WRITE = 1;
  int count = COUNT;

  if (fork() == 0) {

    void* buff = malloc(SIZE);
    memset(buff, 'a', SIZE);
    while(count--) {
        write(p[WRITE], buff, SIZE);
    }
  } else {

    int st, ed; // time count
    st = uptime(); // time count : start

    void* buff2 = malloc(SIZE);
    while(count--) {
      read(p[READ], buff2, SIZE);
    }

    ed = uptime(); // time count : end
    printf("[pipe] %d times in pipes with %d time slides\n", COUNT, ed - st);
 }
 exit(0);
}
