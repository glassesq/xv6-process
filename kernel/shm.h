// shared memory
#define NSHM 256
#define NSHM_IN_PROC 8
struct sharedmemory
{
  void *addr;
  int token;
  int count;
  // TODO: optional creator
};
