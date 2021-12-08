// semaphore by lsr
struct semaphore {
    struct spinlock lock; 
    int count;
    int allocated; 
};

extern struct semaphore sems[NSEMS];