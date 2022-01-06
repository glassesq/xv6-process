#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "shm.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

struct sharedmemory shmlist[NSHM];
struct spinlock shmlock;

struct shm_result {
  int found;
  int extra;
};

// 新建一个shared memory
void create_shm(int id, int token) {
  shmlist[id].token = token;
  shmlist[id].count = 1;
  shmlist[id].addr = kalloc();
  printf("shared memory %d: created.\n", id);
}

// 插入一个shared memory
void add_shm(int id) {
  shmlist[id].count ++;
  printf("shared memory %d: add one. now have %d\n", id, shmlist[id].count);
}

// 在全局shmlist中寻找token或空位
void shmfindinglobal(int token, struct shm_result* result) {
  int i = 0;
  result->found = -1;
  result->extra = -1;
  for (i = 0; i < NSHM; i++) {
    if (shmlist[i].token != token) {
      if (result->extra < 0 && shmlist[i].token < 0) {
        result->extra = i;
      }
      continue;
    }
    else {
      result->found = i;
    }
  }
  return;
}

// 在进程的shmlist中寻找token或空位
void shmfindinproc(int token, struct shm_result* result) {
 struct proc *current = myproc();
  int i = 0;
  result->found = -1;
  result->extra = -1;
  for (i = 0; i < NSHM_IN_PROC; i++) {
    if (current->privateshmlist[i] != token) {
      if (result->extra < 0 && current->privateshmlist[i] < 0) {
        result->extra = i;
      }
      continue;
    }
    else {
      result->found = i;
    }
  }
  return;
}

void shminit(void) {
  printf("shminit: say hi\n");
  int i;
  for (i = 0; i < NSHM; i++) {
    shmlist[i].addr = 0;
    shmlist[i].token = -1;
    shmlist[i].count = 0;
  }
}

int shmget(int token) {
  printf("shmget: say hi\n");
  acquire(&shmlock);  // 获取shmlock
  if (token < 0) {
    printf("wrong token for shget");
    release(&shmlock);  // 释放shmlock
    return -1;
  }

  struct shm_result result;
  shmfindinproc(token, &result);
  if( result.found < 0  && result.extra >= 0) {
    struct shm_result global_result;
    shmfindinglobal(token, &global_result);
    if( global_result.found >= 0 ) {
      struct proc *current = myproc();
      current->privateshmlist[result.extra] = token;
      add_shm(global_result.found);
    }
    else if( global_result.found <= 0 ) {
      struct proc *current = myproc();
      current->privateshmlist[result.extra] = token;
      create_shm(global_result.extra, token);
    }
    else {
      printf("no more space for shm in global");
      release(&shmlock);
      return -1;
    }
    release(&shmlock);
    return 0;
  }
  else {
    if( result.found > 0 ) {
      printf("token already exist");
    }
    else if( result.extra < 0 ) {
      printf("no more space for shm in current process");
    }
    release(&shmlock);
    return -1;
  } 
}
