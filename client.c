#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int ft_strlen(char *string)
{
	int count;

	count = 0;
	while (string[count])
		count++;
	return (count);
}

char *char_to_bin(int c)
{
	char	*bin;
	int		i;

	bin = malloc(sizeof(char) * 9);
	if (!bin)
		return (NULL);
	i = 7;
	while (i >= 0 && c != 0)
	{
		if (c % 2 == 1)
			bin[i] = '1';
		else
			bin[i] = '0';
		c /= 2;
		i--;
	}
	while (i >= 0)
		bin[i--] = '0';
	bin[8] = '\0';
	return (bin);
}

int send_signals(int pid, char *bin)
{
	int state;

	state = 0;
	while (*bin && state >= 0)
	{
		if (*bin == '0')
			state = kill(pid, SIGUSR1);
		else
			state = kill(pid, SIGUSR2);
		bin++;
		usleep(10);
	}
	return (state);
}

int error(char *cause)
{
	while (*cause)
		write(STDOUT_FILENO, &*cause++, 1);
	return (1);
}

int	main(int argc, char **argv)
{
	int		state;
	int 	pid;
	char	*string;
	char	 *test_bin;

	if (argc != 3)
		return (error("Error: Invalid Argument\n"));
	string = argv[2];
	pid = atoi(argv[1]);
	state = 0;
	while (*string && state >= 0)
	{
		test_bin = char_to_bin(*string);
		state = send_signals(pid, test_bin);
		free(test_bin);
		string++;
	}
	if (state >= 0)
		state = send_signals(pid, "00000000");
	if (state == -1)
		return (error("Error: Can't send signals\n"));
	return (0);
}
