/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:24:41 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/24 19:28:04 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// Función que libera memoria de un array de strings
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	handle_permission_error(char *cmd, char *path, char **args)
{
	ft_printf("pipex: %s: Permission denied\n", cmd);
	free(path);
	free_array(args);
	exit(126);
}

static void	handle_not_found(char *cmd, char *path, char **args)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 21);
	free_array(args);
	free(path);
	exit(127);
}

static void	handle_path_error(char *cmd, char *path, char **args)
{
	if ((cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0
			|| ft_strncmp(cmd, "../", 3) == 0)
		&& access(cmd, F_OK) == 0)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": permission denied\n", 21);
		free_array(args);
		free(path);
		exit(126);
	}
	else
		handle_not_found(cmd, path, args);
}

void	exec(char *argv, char *env[])
{
	char	*path;
	char	**args;

	args = ft_split(argv, ' ');
	if (!args || !args[0])
	{
		free_array(args);
		write(2, "pipex: command not found\n", 25);
		exit(127);
	}
	path = find_path(args[0], env);
	if (path == NULL)
		handle_path_error(args[0], path, args);
	if (access(path, X_OK) != 0)
		handle_permission_error(args[0], path, args);
	execve(path, args, env);
	ft_printf("pipex: %s: %s\n", args[0], strerror(errno));
	free(path);
	free_array(args);
	exit(EXIT_FAILURE);
}
