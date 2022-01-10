#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// syscalls of semaphore by lsr
uint64
sys_semalloc(void)
{
  int nsem;

  if (argint(0, &nsem) < 0) 
    return -1;

  return semalloc(nsem);
}

uint64
sys_semfree(void)
{
  int semid;

  if (argint(0, &semid) < 0)
    return -1;

  semfree(semid);

  return 0;
}

uint64 
sys_semwait(void)
{
  int sem;
  int count;

  if (argint(0, &sem) < 0)
    return -1;
  if (argint(1, &count) < 0)
    return -1;

  semwait(sem, count);

  return 0;
}

uint64
sys_semsignal(void)
{
  int sem;
  int count;

  if (argint(0, &sem) < 0)
    return -1;
  if (argint(1, &count) < 0)
    return -1;

  semsignal(sem, count);

  return 0;
}

// get a new shared memory
uint64
sys_shmget(void) 
{
  int token, method;
  if (argint(0, &token) < 0 || argint(1, &method)) 
    return -1;
  int ret = shmget(token, method);
  return ret;
}

// delete an old shared memory
uint64
sys_shmdel(void) 
{
  int token;
  if (argint(0, &token) < 0)
    return -1;
  int ret = shmdel(token);
  return ret;
}

// read from shared memory
uint64
sys_shmread(void) 
{
  int token, length;
  uint64 buffer, addr;
  if (argint(0, &token) < 0 || argaddr(1, &addr) < 0 || argaddr(2, &buffer) < 0 || argint(3, &length) < 0)
    return -1;
  int ret = shmread(token, addr, buffer, length);
//  printf("buffer:%p addr:%p\n", buffer, addr);
  return ret;
}

// write from shared memory
uint64
sys_shmwrite(void) 
{
  int token, length;
  uint64 pro_addr, addr;
  if(argint(0, &token) < 0 || argaddr(1, &addr) < 0 ||  argaddr(2, &pro_addr) || argint(3, &length) < 0)
    return -1;
  void* kbuff = kalloc();
  fetchstr(pro_addr, kbuff, length);
//  printf("pro_addr:%p real_addr:%p addr:%p\n", pro_addr, kbuff, addr);
  int ret = shmwrite(token, addr, (uint64)kbuff, length);
  kfree(kbuff);
  return ret;
}