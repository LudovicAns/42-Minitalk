/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lanselin <lanselin@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:49:39 by lanselin          #+#    #+#             */
/*   Updated: 2021/08/25 18:49:39 by lanselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	handler_sigusr(int signum)
{
	static int	bitshift = 0;
	static char	byte = 0xFF;

	if (signum == SIGUSR1)
	{
		printf("0");
		byte ^= 0x80 >> bitshift;
	}
	else if (signum == SIGUSR2)
	{
		printf("1");
		byte |= 0x80 >> bitshift;
	}
	bitshift++;
	if (bitshift == 8)
	{
		printf(" -> %c\n", byte);
		byte = 0xFF;
		bitshift = 0;
	}
}

/*
*	Welcome to the main function of server.
*
*	@return	nothing
*/
int	main(void)
{
   pid_t		pid;

	pid = getpid();
	printf("PID: %d\n", pid);
   	signal(SIGUSR1, handler_sigusr);
   	signal(SIGUSR2, handler_sigusr);
   	while (1)
		pause();
}
