/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:24:41 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/18 13:17:07 by mgrandia         ###   ########.fr       */
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

//funcion que retorna un char ** con todos los path encontrados por separado
/*static char	**path(char *env[])
{
	int		i;
	char	*path_value;
	char	**paths;

	i = 0;
	path_value = NULL;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			path_value = env[i] + 5;
		i++;
	}
	if (path_value == NULL)
		return (NULL);
	paths = ft_split(path_value, ':');
	return (paths);
}

static char	*check_direct_path(char *cmd)
{
	if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0)
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*exec1;
	char	*exec2;

	i = 0;
	while (paths[i])
	{
		exec1 = ft_strjoin(paths[i], "/");
		exec2 = ft_strjoin(exec1, cmd);
		free(exec1);
		if (access(exec2, F_OK) == 0)
		{
			free_array(paths);
			return (exec2);
		}
		free(exec2);
		i++;
	}
	free_array(paths);
	return (NULL);
}

static char	*find_path(char *cmd, char *env[])
{
	char	**paths;
	char	*direct_path;

	direct_path = check_direct_path(cmd);
	if (direct_path)
		return (direct_path);
	paths = path(env);
	if (paths == NULL)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
*/
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
