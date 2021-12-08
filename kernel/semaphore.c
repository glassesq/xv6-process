#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "semaphore.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

struct semaphore sems[NSEMS];
// allocate a semaphore
int 
semalloc(int nsem) 
{
  int i;
  for (i = 0; i < NSEMS; ++i)
  {
    acquire(&sems[i].lock);
    if (sems[i].allocated == 0)
    {
      sems[i].allocated = 1;
      sems[i].count = nsem;
      return i;    
    }
    release(&sems[i].lock);
  }
  return -1;
}

// free sems[id]
void 
semfree(int id)
{
  acquire(&sems[id].lock);
  sems[id].allocated = 0;
  release(&sems[id].lock);
}

// P operation
void 
semwait(int id, int nsem)
{
  acquire(&sems[id].lock);
  while(sems[id].count < nsem)
    sleep(&sems[id], &sems[id].lock);
  sems[id].count -= nsem;
  release(&sems[id].lock);
}

// V operation
void 
semsignal(int id, int nsem)
{
  acquire(&sems[id].lock);
  sems[id].count += nsem;
  wakeup1p(&sems[id]);
  release(&sems[id].lock);
}