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

/*
*	The exit_error function is called when program need to exit due to error 
*	(yeah, it's obvious).
*
*	@param	int c_pid	-	The pid of client to tell him that server has
						encountered an error.
*
*	@param	char *message	-	The string who was malloced in handler_sigusrs
*							function (prevent potential leak ^^).
*
*	@result	Nothing.
*/
void	exit_error(int c_pid, char *message)
{
	if (message)
		free(message);
	kill(c_pid, SIGUSR2);
	exit(EXIT_FAILURE);
}

/*
*	This function is here to make handler_sigusrs function less bigger. 
*	It display the recieved message and free it after.
*
*	@param	char *message	-	The message to display in STDOUT.
*
*	@result	NULL.
*/
char	*disp_string(char *message)
{
	ft_putstr_fd(message, 1);
	free(message);
	return (NULL);
}

/*
*	The handler_sigusrs function handle signals of the program. 
*	Depend of recieved signal (SIGUSR1 or SIGUSR2), the function 
*	build the recieved message bit per bit, byte per byte.
*	When it recieve NULL byte, the function call disp_string function.
*
*	@param	int signum	-	Received signal value.
*
*	@param	siginfo_t *info	-	Contains usefull informations coming from 
*							client.
*
*	@param	void *context	-	Unused but compulsory due to sigaction 
*							flag (SA_SIGINFO).
*
*	@result	Nothing.
*/
void	handler_sigusrs(int signum, siginfo_t *info, void *context)
{
	static int	bitshift = 0;
	static char	byte = 0xFF;
	static char	*message = 0;
	static int	c_pid;

	(void)context;
	if (info->si_pid)
		c_pid = info->si_pid;
	if (signum == SIGUSR1)
		byte ^= 0x80 >> bitshift;
	else if (signum == SIGUSR2)
		byte |= 0x80 >> bitshift;
	if (++bitshift == 8)
	{
		if (byte)
			message = ft_strjoin_char(message, byte);
		else
			message = disp_string(message);
		byte = 0xFF;
		bitshift = 0;
	}
	if (kill(c_pid, SIGUSR1) == -1)
		exit_error(c_pid, message);
}

/*
*	Welcome to the main function of server.
*
*	@return	Nothing.
*/
int	main(void)
{
	struct sigaction	sa_signal;
	sigset_t			block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sa_signal.sa_handler = 0;
	sa_signal.sa_mask = block_mask;
	sa_signal.sa_flags = SA_SIGINFO;
	sa_signal.sa_sigaction = handler_sigusrs;
	sigaction(SIGUSR1, &sa_signal, NULL);
	sigaction(SIGUSR2, &sa_signal, NULL);
	ft_putstr_color_fd(ANSI_COLOR_BLUE, "PID of this server is ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putchar_fd('\n', 1);
	while (1)
		pause();
}
