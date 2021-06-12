#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//Todo: Gérer les signaux qui peuvent se superposer.

int g_count = 0;

void sigusr1_handler(int num)
{
	write(STDOUT_FILENO, "0", 1);
	g_count++;
}

void sigusr2_handler(int num)
{
	write(STDOUT_FILENO, "1", 1);
	g_count++;
}

int	main(void)
{
	int pid = getpid();
	printf("Starting server with PID : %d\n\n", pid);
	printf("Listning for :\n");

	printf("  - SIGUSR1 (%d)\n", SIGUSR1);
	signal(SIGUSR1, sigusr1_handler);
	printf("  - SIGUSR2 (%d)\n", SIGUSR2);
	signal(SIGUSR2, sigusr2_handler);
	printf("\n");
	// Infinite loop, waiting for any signals.
	while (1)
	{
		if (g_count == 8)
		{
			g_count = 0;
			write(STDOUT_FILENO, "\n", 1);
		}
		pause();
	}
	return (0);
}
