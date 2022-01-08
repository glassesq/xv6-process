#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char **argv)
{
	printf("shmtest: say hi\n");
	int ret = shmget(1);
	printf("test_user_shmget: return %d\n", ret);
	ret = shmdel(1);
	printf("test_user_shmdel: return %d\n", ret);
	// Exit
	exit(0);
}
