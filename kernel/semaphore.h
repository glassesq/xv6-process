// semaphore
struct semaphore {
    struct spinlock; 
    int count;
    int allocated; 
};

extern struct semaphore sems[NSEMS];