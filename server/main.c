/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/14 03:56:12 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 23:03:45 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "serveur.h"

static int				usage(char *name)
{
	fprintf(stderr, "%sUsage: %s <port>%s\n", RED, name, END);
	return (EXIT_FAILURE);
}

static void				send_file(int cs, char *f_name)
{
	int					fd;
	ssize_t				len;
	uint32_t			size;
	char				buf[BUFF_SIZE];

	if ((fd = open(f_name, O_RDONLY)) == -1 && safe_send(cs, "KO", 2))
		return ;
	safe_send(cs, "OK", 2) && (size = htonl(get_file_size(fd)));
	write(cs, (void*)&size, sizeof(size)) && (size = ntohl(size));
	while (size > 0 && (len = read(fd, buf, BUFF_SIZE)) > 0 &&
			safe_send(cs, buf, (size_t)len))
		size -= len;
	printf("%sUser [%d] downloaded [%s]%s\n", BLUE, cs, f_name, END);
	close(fd);
}

static void				recv_file(int cs, char *f_name)
{
	int					fd;
	uint32_t			size;
	ssize_t				len;
	char				buf[BUFF_SIZE];

	if ((fd = open(f_name, O_TRUNC | O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG |
					S_IRWXO)) == -1 && safe_send(cs, "KO", 2))
		return ;
	safe_send(cs, "OK", 2) && read(cs, (void*)&size, sizeof(size));
	size = ntohl(size);
	while (size > 0 && (len = read(cs, buf, BUFF_SIZE)) > 0 &&
		write(fd, buf, (size_t)len))
		size -= len;
	printf("%sUser [%d] uploaded [%s]%s\n", RGREEN, cs, f_name, END);
	close(fd);
}

static void				handle_client(int cs, struct sockaddr_in *csin)
{
	ssize_t				len;
	char				buf[BUFF_SIZE];

	printf("%s[%s] connected on socket [%d]%s\n", GREEN,
			inet_ntoa(csin->sin_addr), cs, END);
	while ((len = read(cs, buf, BUFF_SIZE - 1)) > 0)
	{
		buf[len] = '\0';
		if (!ft_strcmp(buf, "quit") && !close(cs) &&
			printf("%s[%s] just left%s\n", LIGHT_RED,
					inet_ntoa(csin->sin_addr), END))
			exit(EXIT_SUCCESS);
		else if (!ft_strncmp(buf, "get ", 4))
			send_file(cs, &buf[4]);
		else if (!ft_strncmp(buf, "put ", 4))
			recv_file(cs, &buf[4]);
		else if (!ft_strcmp(buf, "pwd"))
			show_pwd(cs);
		else if (!ft_strcmp(buf, "ls"))
			show_ls(cs);
		else if (!ft_strncmp(buf, "cd ", 3))
			change_dir(cs, &buf[3]);
		else if (!ft_strncmp(buf, "mkdir ", 6) || !ft_strncmp(buf, "rmdir ", 6))
			handle_dir(cs, buf);
	}
}

int						main(int ac, char **av)
{
	int					cs;
	int					sock;
	unsigned int		cslen;
	struct sockaddr_in	sin;
	struct sockaddr_in	csin;

	if (ac != 2)
		return (usage(av[0]));
	get_initial_path(1);
	sock = init_socket();
	sin.sin_family = AF_INET;
	sin.sin_port = htons(ft_atoi(av[1]));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	ft_memset(&sin.sin_zero, 0, sizeof(sin.sin_zero));
	if (!bind(sock, (struct sockaddr*)&sin, sizeof(sin)))
	{
		!listen(sock, 42) && printf("Listenting on port [%s]...\n", av[1]);
		signal(SIGCHLD, SIG_IGN);
		while ((cs = accept(sock, (struct sockaddr*)&csin, &cslen)) != -1)
			if (!fork())
				handle_client(cs, &csin);
	}
	printf("%sCouldn't listen on port: [%d]%s\n", RED,
			ft_atoi(av[1]), END);
	return (close(sock));
}
