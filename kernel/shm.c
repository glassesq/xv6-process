#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "shm.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

#define SHM_READ 0
#define SHM_WRITE 1

struct sharedmemory shmlist[NSHM];
struct spinlock shmlock;

struct shm_result {
  int found;
  int extra;
};

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
//  printf("global: token %d found in %d\n", token, result->found);
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
//  printf("in-proc: token %d found in %d\n", token, result->found);
  return;
}

// 新建一个shared memory
void create_shm(int id, int token) {
  shmlist[id].token = token;
  shmlist[id].count = 1;
  shmlist[id].addr = kalloc();
//  printf("shared memory %d: created in %d.\n", shmlist[id].token, id);
}

// 插入一个shared memory
void add_shm(int id) {
  shmlist[id].count ++;
  printf("shared memory %d: add one. now have %d\n", id, shmlist[id].count);
}

// 在全局删除一个名为token的shm
void delete_shm(int token) {
//  printf("test_delete_shm: say hi\n");
  struct shm_result result;
  shmfindinglobal(token, &result);
//  printf("test_delete_shm: token %d found in %d\n", token, result.found);
  shmlist[result.found].count--;
  if(shmlist[result.found].count == 0 ) {
    shmlist[result.found].token = -1;
    kfree(shmlist[result.found].addr);
  }
}

void shminit(void) {
//  printf("test_shminit: say hi\n");
  int i;
  for (i = 0; i < NSHM; i++) {
    shmlist[i].addr = 0;
    shmlist[i].token = -1;
    shmlist[i].count = 0;
  }
}

// 获取一个新的shared memory, 指定token
int shmget(int token) {
//  printf("test_shmget: say hi\n");
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
//      printf("token: %d %d\n", current->privateshmlist[0], shmlist[0].token);
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

int shmdel(int token) {
//  printf("test_shmdel: say hi\n");
  acquire(&shmlock);
  if (token < 0) {
    printf("wrong token for shget");
    release(&shmlock);  // 释放shmlock
    return -1;
  }

//  printf("test_shmdel: try to get result\n");
  struct shm_result result;
  shmfindinproc(token, &result);
  if( result.found < 0 ) {
    printf("no shm of this token in process");
    release(&shmlock);  // 释放shmlock
    return -1;
  }
//  printf("test_shmdel: found in proc\n");
  struct proc * current = myproc();
  delete_shm(token);
  current->privateshmlist[result.found] = -1;
  release(&shmlock);  // 释放shmlock
  return 0;
}

void shmdelall(struct proc* p) {
  acquire(&shmlock);
//  printf("test_shmdelall: say hi\n");
  int count = 0;
  int i = 0;
  for (i = 0; i < NSHM_IN_PROC; i++) {
    if (p->privateshmlist[i] >= 0) {
      delete_shm(p->privateshmlist[i]);
      ++count;
    }
  }
//  printf("test_shmdelall: delete %d shms, say bye\n", count);
  release(&shmlock);
}

int shmcheck(int token, int type) {
  struct shm_result result;
  shmfindinproc(token, &result);
  if( result.found < 0 ) return 0;
  // TODO: check read & write permission
  return 1;
}

int shmread(int token, uint64 addr, uint64 buffer, int length) {
  acquire(&shmlock);
//  printf("test_shmread: say hi\n");
  if( !shmcheck(token, SHM_READ) ) {
    release(&shmlock);
    return -1;
  }
  struct shm_result result;
  shmfindinglobal(token, &result);
  void* src = addr + shmlist[result.found].addr;
  struct proc* p = myproc();
  copyout(p->pagetable, buffer, (char*)src, length);
  release(&shmlock);
  return 0;
}

int shmwrite(int token, uint64 addr, uint64 buffer, int length) {
  acquire(&shmlock);
//  printf("test_shmwrite: say hi\n");
  if( !shmcheck(token, SHM_WRITE) ) {
    release(&shmlock);
    return -1;
  }
  struct shm_result result;
  shmfindinglobal(token, &result);
  void* src = addr + shmlist[result.found].addr;
  memmove((void*)src, (const void*)buffer, length);
  release(&shmlock);
  return 0;
}