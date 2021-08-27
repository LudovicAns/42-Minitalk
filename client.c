/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lanselin <lanselin@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:57:14 by lanselin          #+#    #+#             */
/*   Updated: 2021/08/25 18:57:14 by lanselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
*	The exit_error function is called when program need to exit due to error
*	(yeah, it's obvious).
*
*	@param	char *message	-	The string who was malloced in send_bit
*							function (prevent potential leak ^^).
*
*	@result	-	Nothing.
*/
void	exit_error(char *message)
{
	if (message)
		free(message);
	exit(EXIT_FAILURE);
}

/*
*	The send_bitnull function is called when send_bit function has finished
*	to send all bits in message.
*	It permit to server to know when the message is over from this client.
*
*	@param	int s_pid	-	pid of the server we want to communic with.
*
*	@param char *message	-	The string who was malloced in send_bit
*							function (prevent potential leak ^^).
*
*	@result	-	0 if byte is not totaly send else 1.
*/
int	send_bitnull(int s_pid, char *message)
{
	static int	count = 0;

	if (count != 8)
	{
		if (kill(s_pid, SIGUSR1) == -1)
			exit_error(message);
		count++;
		return (0);
	}
	return (1);
}

/*
*	This function send bit per bit all bytes present in the string. The
*	program wait for a signal from the server to send the others
*	bits.
*
*	@param	int pid	-	pid of the server we want to communic with.
*					This parameter is given not null only one time 
*					in the main function.
*
*	@param char *str	-	This is the string to send to the server.
*						As pid, this parameter is given not null
*						only one time in the main function.
*
*	@result	-	0 if we are waiting signal from server or 1 to end the
*			program.
*/
int	send_bit(int pid, char *str)
{
	static char	*message = 0;
	static int	bitshift = -1;
	static int	s_pid = 0;
	
	if (str)
		message = ft_strdup(str);
	if (!message)
		exit_error(0);
	if (pid)
		s_pid = pid;
	if (message[++bitshift / 8])
	{
		if (message[bitshift / 8] & (0x80 >> bitshift % 8))
		{
			if (kill(s_pid, SIGUSR2) == -1)
				exit_error(message);
		}
		else if (kill(s_pid, SIGUSR1) == -1)
			exit_error(message);
		return (0);
	}
	if (!send_bitnull(s_pid, message))
		return (0);
	free(message);
	return (1);
}

/*
*	The handler_sigusrs handle two different signals.
*	
*	SIGUSR1	-	Server is waiting for other bit.
*	SIGUSR2	-	Server encountered a problem.
*
*	@param	int signum	-	Received signal value.
*
*	@result	-	Nothing.
*/
void	handler_sigusrs(int signum)
{
	int	end;

	end = 0;
	if (signum == SIGUSR1)
		end = send_bit(0, 0);
	else if (signum == SIGUSR2)
		exit(EXIT_FAILURE);
	if (end)
		exit(EXIT_SUCCESS);
}

/*
*	Welcome to the main function of client.
*
*	@param	int	argc	-	Argument count.
*	@param	char **argv	-	Parameters given to client by user.
*
*	@return	-	Nothing.
*/
int	main(int argc, char **argv)
{
	if (argc != 3 || !ft_str_isnumeric(argv[1]))
		exit(EXIT_FAILURE);

	signal(SIGUSR1, handler_sigusrs);
	signal(SIGUSR2, handler_sigusrs);
	send_bit(ft_atoi(argv[1]), argv[2]);
	while (1)
		pause();
}
