/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 19:00:24 by fmorales          #+#    #+#             */
/*   Updated: 2015/12/30 19:00:27 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "client.h"

void			change_dir(char *path)
{
	char		*tmp;

	tmp = getcwd(NULL, 0);
	if (chdir(path) == 0)
	{
		free(tmp);
		return ;
	}
	else
	{
		printf("%sERROR ACCESSING FOLDER [%s]%s\n", LIGHT_RED, path, END);
		free(tmp);
		return ;
	}
}

void			show_pwd(void)
{
	char		*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr(pwd);
	ft_putstr("\n");
	free(pwd);
}

void			show_ls(void)
{
	pid_t		child;
	static char	*av[] = {"ls", "-lF", NULL};

	if (!(child = fork()))
		execv("/bin/ls", av);
	wait4(child, NULL, 0, NULL);
}

void			handle_dir(int sock, char *s)
{
	char		buf[2];

	safe_send(sock, s, ft_strlen(s));
	read(sock, buf, 2);
	if (buf[0] == 'K')
		printf("%sERROR HANDLING FOLDER %s %s\n", LIGHT_RED, &s[6], END);
	else
		printf("%s[SUCCESS]%s\n", GREEN, END);
}

int				check_client_commands(int sock, char *s)
{
	if (!ft_strcmp(s, "lpwd"))
	{
		show_pwd();
		return (1);
	}
	else if (!ft_strcmp(s, "lls"))
	{
		show_ls();
		return (1);
	}
	else if (!ft_strncmp(s, "lcd ", 4))
	{
		change_dir(&s[4]);
		return (1);
	}
	else if (!ft_strncmp(s, "mkdir ", 6) || !ft_strncmp(s, "rmdir ", 6))
	{
		handle_dir(sock, s);
		return (1);
	}
	return (0);
}
