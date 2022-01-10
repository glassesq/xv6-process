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
  printf("shmtest: say hi\n");
  if(argc != 3) {
    printf("wrong arguments\n");
    exit(0);
  }
  int COUNT = atoi(argv[1]);
  int SIZE = atoi(argv[2]);

  int sem = semalloc(0);
  int count = COUNT;

  if (fork() == 0) {
    shmget(1);

    void* buff = malloc(SIZE);
    memset(buff, 'a', SIZE);

//    write(1, "write to shared memory:", 23);
//    write(1, buff, 5);
    while(count--) {
      shmwrite(1, 0, buff, SIZE);
      semsignal(sem, 1);
    }
//		write(1, "\n------END CHILD-------\n", 24);
		semsignal(sem, 1);
    shmdel(1);
  } else {
		shmget(1);
    void* buff2 = malloc(SIZE);
    int st, ed;
    st = uptime();
    while(count--) {
		  semwait(sem, 1);
      shmread(1, 0, buff2, SIZE);
    }
    ed = uptime();
    printf("[shared memory] %d times in pipes with %d time slides\n", COUNT, ed - st);
//		write(1, "read from shared memory:", 24);
//    write(1, buff2, 5);
//		write(1, "\n------END PARENT-------\n", 24);
    shmdel(1);
 }
  semfree(sem);
  // Exit
  exit(0);
}
