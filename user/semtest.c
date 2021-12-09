#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#define NCHILD 2
#define TARGET_COUNT 50
#define COUNTER_FILE "counter"

int semid;

int counter_init(char *filename, int value)
{
	int fd;

	if ((fd = open(filename, O_CREATE | O_RDWR)) < 0) {
		fprintf(1, "counter_init: error initializing file: %s\n", filename);
		exit(1);
	}

	fprintf(fd, "%d\n", value);
	close(fd);

	return 0;
}

int counter_get(char *filename)
{
	int fd, n, value;
	char buffer[32];
	if ((fd = open(filename, O_CREATE | O_RDWR)) < 0) {
		fprintf(1, "counter_get: error opening file: %s\n", filename);
		exit(1);
	}
	n = read(fd, buffer, 31);
	buffer[n] = '\0';
	value = atoi(buffer);
	close(fd);

	return value;
}

int counter_set(char *filename, int value)
{
	int fd;

	if ((fd = open(filename, O_CREATE | O_RDWR)) < 0) {
		fprintf(1, "counter_set: error opening file: %s\n", filename);
		exit(1);
	}

	fprintf(fd, "%d\n", value);
	close(fd);

	return value;
}

void child(void)
{
	int i;
	int counter;

	fprintf(1, "Process started...\n");
	sleep(10);

	for (i=0; i<TARGET_COUNT; i++) {
		semwait(semid, 1);
		
		counter = counter_get("counter");
		counter ++;
		counter_set("counter", counter);

		semsignal(semid, 1);
	}

	exit(0);
}

int main(int argc, char **argv)
{
	int i;
	int nsem;
	int final_counter;
	int final_target = NCHILD * TARGET_COUNT;

	if (argc >= 2)
		nsem = NCHILD;
	else
		nsem = 1;

	// Initialize semaphore to 1
	if ( (semid = semalloc(nsem)) < 0)
	{
		fprintf(1, "main: error initializing semaphore\n");
		exit(1);
	}

	fprintf(1, "main: initialized semaphore %d to %d\n", semid, nsem);

	// Initialize counter
	counter_init(COUNTER_FILE, 0);

	fprintf(1, "Running with %d processes...\n", NCHILD);

	// Start all children
	for (i=0; i<NCHILD; i++) {
		int pid = fork();
		if (pid == 0)
			child();
	}
	
	// Wait for all children
	for (i=0; i<NCHILD; i++) 
		wait(0);

	// Check the result
	final_counter = counter_get(COUNTER_FILE);
	fprintf(1, "Final counter is %d, target is %d\n", final_counter, final_target);
	if (final_counter == final_target)
		fprintf(1, "TEST PASSED!\n");
	else
		fprintf(1, "TEST FAILED!\n");
	
	// Clean up semaphore
	semfree(semid);
	
	// Exit
	exit(0);
}
