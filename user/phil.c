#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define COUNT 1
#define FILENAME "phil.out"
int state[N], s[N];
int mutex;

void state_get(char *filename)
{
	int fd, i;
	char buffer[N * 2];
	if ((fd = open(filename, O_CREATE | O_RDWR)) < 0) {
		fprintf(1, "state_get: error opening file: %s\n", filename);
		exit(1);
	}
	read(fd, buffer, N);

    for (i = 0; i < N; ++i) {
        state[i] = buffer[i] - '0';
    }

	close(fd);
}

void state_set(char *filename)
{
	int fd, i;

	if ((fd = open(filename, O_CREATE | O_RDWR)) < 0) {
		fprintf(1, "state_set: error opening file: %s\n", filename);
		exit(1);
	}

    for (i = 0; i < N; ++i) {
        fprintf(fd, "%c", state[i] + '0');
    }
	close(fd);
}

void test(int i){
    state_get(FILENAME);
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) 
    {
        fprintf(1, "philosopher %d start eating\n", i);
        state[i] = EATING;
        state_set(FILENAME);
        semsignal(s[i], 1);
    }
}

void take_forks(int i) 
{
    semwait(mutex, 1);
    fprintf(1, "philosopher %d is hungry\n", i);
    state_get(FILENAME);
    state[i] = HUNGRY;
    state_set(FILENAME);
    test(i);
    semsignal(mutex, 1);
    semwait(s[i], 1);
}

void put_forks(int i)
{
    semwait(mutex, 1);
    fprintf(1, "philosopher %d start thinking\n", i);
    state_get(FILENAME);
    state[i] = THINKING;
    state_set(FILENAME);
    test(LEFT);
    test(RIGHT);
    semsignal(mutex, 1);
}

void philosopher(int i) 
{   
    int cnt = COUNT;
    while(cnt--)
    {
        take_forks(i);
        put_forks(i);
    }
    exit(0);
}


int main(int argc, char **argv)
{
    int i;
    for(i = 0; i < N; ++i)
        s[i] = semalloc(0);
    mutex = semalloc(1);
    state_set(FILENAME);
    for (i = 0; i < N; ++i)
    {
        int pid = fork();
        if (pid == 0)
            philosopher(i);
    }

    for (i = 0; i < N; ++i)
    {
        wait(0);
    }

	for(i = 0; i < N; ++i)
        semfree(s[i]);
    semfree(mutex);
	
	exit(0);
}
