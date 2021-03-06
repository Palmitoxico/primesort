/*
 * Primesort
 */
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int isPrime(int64_t num)
{
	if (num == 0) return 1;
	if (num == 1) return 0;
	if (num == 2) return 1;
	if (num % 2 == 0) return 0;
	for (int64_t div = 3; div <= num/2; div += 2)
	{
		if (num % div == 0) return 0;
	}
	return 1;
}

int main()
{
	int64_t num;
	int sort_pipe[2];
	pid_t sort_pid = -1;

	pipe(sort_pipe);
	sort_pid = fork();

	if (sort_pid == 0)
	{
		/*
		 * Child
		 */
		dup2(sort_pipe[0], STDIN_FILENO);
		close(sort_pipe[1]);
		execlp("sort", "sort", "-n", "-r", NULL);
		printf("Child error!\n");
		return -1;
	}

	close(sort_pipe[0]);

	while (scanf("%l" PRId64 "\n", &num) != EOF)
	{
		if (num < 0) break;
		if (!isPrime(num))
		{
			char strnum[16];
			int strsize = snprintf(strnum, sizeof(strnum), "%l" PRId64 "\n", num);
			write(sort_pipe[1], strnum, strsize);
		}
	}

	close(sort_pipe[1]);
	wait(NULL);
	return 0;
}
