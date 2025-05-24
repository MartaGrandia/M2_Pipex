/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:24:41 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/18 13:17:07 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	first_child(char *argv[], char *env[], int *fd_pipex)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_printf("pipex: %s: %s\n", strerror(errno), argv[1]);
		exit(EXIT_FAILURE);
	}
	dup2(fd_pipex[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd_pipex[0]);
	close(fd_pipex[1]);
	close(fd);
	exec(argv[2], env);
	exit(EXIT_FAILURE);
}

static void	second_child(char *argv[], char *env[], int *fd_pipex)
{
	int	fd;

	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("pipex: %s: %s\n", strerror(errno), argv[4]);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd_pipex[0], STDIN_FILENO);
	close(fd_pipex[0]);
	close(fd_pipex[1]);
	close(fd);
	exec(argv[3], env);
	exit(EXIT_FAILURE);
}

static	void	ft_error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

static int	pipex(char *argv[], char *env[])
{
	int		fd_pipex[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(fd_pipex) == -1)
		ft_error();
	pid1 = fork();
	if (pid1 == -1)
		ft_error();
	if (pid1 == 0)
		first_child(argv, env, fd_pipex);
	pid2 = fork();
	if (pid2 == -1)
		ft_error();
	if (pid2 == 0)
		second_child(argv, env, fd_pipex);
	close(fd_pipex[0]);
	close(fd_pipex[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if ((status & 0x7F) == 0)
		return ((status >> 8) & 0xFF);
	return (EXIT_FAILURE);
}

int	main(int argc, char *argv[], char *env[])
{
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile \"cmd1\" \"cmd2\" outfile\n");
		exit(EXIT_FAILURE);
	}
	return (pipex(argv, env));
}
