/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 11:03:06 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 23:09:06 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "client.h"

void			display(uint32_t tot, uint32_t rest)
{
	int			percent;
	uint32_t	transf;
	char		*unit;

	unit = "bytes";
	transf = tot - rest;
	if (tot >= 100000)
	{
		transf /= 1000;
		tot /= 1000;
		unit = "KB";
	}
	if (tot >= 100000)
	{
		transf /= 1000;
		tot /= 1000;
		unit = "MB";
	}
	percent = ((float)(transf + 1) / (float)tot) * 100;
	printf("\r%u/%u %s transfered\t\t[%d%%]", transf, tot, unit, percent);
}

size_t			get_file_size(int fd)
{
	struct stat	s;

	fstat(fd, &s);
	return ((size_t)s.st_size);
}

void			send_cd(int sock, char *s)
{
	char		buf[2];

	safe_send(sock, s, ft_strlen(s));
	read(sock, buf, 2);
	if (buf[0] == 'K')
		printf("%sERROR: Permission denied%s\n", LIGHT_RED, END);
	else
		printf("%s[SUCCESS]%s\n", GREEN, END);
}

void			send_pwd(int sock)
{
	size_t		len;
	char		buf[BUFF_SIZE];

	safe_send(sock, "pwd", 3);
	len = read(sock, buf, BUFF_SIZE - 1);
	buf[len] = '\0';
	printf("%s%s%s\n", GREEN, buf, END);
}

void			send_ls(int sock)
{
	int			i;
	size_t		len;
	char		buf[BUFF_SIZE];

	i = 1;
	safe_send(sock, "ls", 2);
	ft_putstr(GREEN);
	while (i && (len = read(sock, buf, BUFF_SIZE - 1)) > 0)
	{
		buf[len] = '\0';
		if (buf[len - 1] == (char)0x04 && !(buf[len - 1] = '\0'))
			i = 0;
		ft_putstr(buf);
	}
	ft_putstr(END);
}
