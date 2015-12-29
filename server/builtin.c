/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 11:03:06 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 22:09:26 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "serveur.h"

size_t			get_file_size(int fd)
{
	struct stat	s;

	fstat(fd, &s);
	return ((size_t)s.st_size);
}

char			*get_initial_path(int flag)
{
	char		*ret;
	static char	*pwd;

	ret = NULL;
	if (flag)
		pwd = getcwd(NULL, 0);
	else
		ret = pwd;
	return (ret);
}

void			change_dir(int cs, char *path)
{
	char		*tmp;
	char		*new;

	tmp = getcwd(NULL, 0);
	if (chdir(path))
	{
		safe_send(cs, "KO", 2);
		free(tmp);
		return ;
	}
	new = getcwd(NULL, 0);
	if (ft_strstr(new, get_initial_path(0)))
		safe_send(cs, "OK", 2);
	else
	{
		safe_send(cs, "KO", 2);
		chdir(tmp);
	}
	free(tmp);
	free(new);
}

void			show_pwd(int cs)
{
	char		*pwd;

	pwd = getcwd(NULL, 0);
	safe_send(cs, pwd, ft_strlen(pwd));
	free(pwd);
}

void			show_ls(int cs)
{
	pid_t		child;
	static char	*av[] = {"ls", "-lF", NULL};

	if (!(child = fork()))
	{
		dup2(cs, STDOUT_FILENO);
		execv("/bin/ls", av);
	}
	wait4(child, NULL, 0, NULL);
	safe_send(cs, "\x04", 1);
}
