/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/14 03:56:12 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 23:13:04 by fmorales         ###   ########.fr       */
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
#include "client.h"

static int				usage(char *name)
{
	fprintf(stderr, "%sUsage: %s <hostname> <port>%s\n", RED, name, END);
	return (EXIT_FAILURE);
}

static void				send_file(int cs, char *s, char *f_name, int fd)
{
	ssize_t				len;
	uint32_t			size;
	uint32_t			size2;
	char				buf[BUFF_SIZE];

	if ((fd = open(f_name, O_RDONLY)) == -1 &&
		printf("%sERROR: No such file [%s]\n%s", LIGHT_RED, f_name, END))
		return ;
	safe_send(cs, s, ft_strlen(s)) && safe_recv(cs, buf, 2);
	if (buf[0] == 'K' && !close(fd) &&
		printf("%sERROR: Permission denied [%s]\n%s", LIGHT_RED, f_name, END))
		return ;
	size = htonl(get_file_size(fd));
	write(cs, (void*)&size, sizeof(size));
	size = ntohl(size);
	size2 = size;
	ft_putstr(RGREEN);
	while (size > 0 && (len = read(fd, buf, BUFF_SIZE)) > 0 &&
			safe_send(cs, buf, (size_t)len))
	{
		size -= len;
		display(size2, size);
	}
	printf("\n%s[%s] uploaded successfully%s\n", BLUE, f_name, END);
	close(fd);
}

static void				recv_file(int cs, char *s, char *f_name, int fd)
{
	uint32_t			size;
	uint32_t			size2;
	ssize_t				len;
	char				buf[BUFF_SIZE];

	if ((fd = open(f_name, O_TRUNC | O_WRONLY | O_CREAT,
					S_IRWXU | S_IRWXG | S_IRWXO)) == -1 &&
		printf("%sERROR: Permission denied [%s]\n%s", LIGHT_RED, f_name, END))
		return ;
	safe_send(cs, s, ft_strlen(s)) && safe_recv(cs, buf, 2);
	if (buf[0] == 'K' && !close(fd) && !unlink(f_name) &&
		printf("%sERROR: No such file [%s]\n%s", LIGHT_RED, f_name, END))
		return ;
	read(cs, (void*)&size, sizeof(size)) && (size = ntohl(size));
	size2 = size;
	ft_putstr(RGREEN);
	while (size > 0 && (len = read(cs, buf, BUFF_SIZE)) > 0 &&
			write(fd, buf, (size_t)len))
	{
		size -= len;
		display(size2, size);
	}
	printf("\n%s[%s] downloaded successfully%s\n", BLUE, f_name, END);
	close(fd);
}

static void				handle_client(int sock, char *s)
{
	ft_putstr(PROMPT);
	while (get_next_line(STDIN_FILENO, &s) > 0)
	{
		if (!ft_strcmp("pwd", s))
			send_pwd(sock);
		else if (!ft_strncmp("cd ", s, 3))
			send_cd(sock, s);
		else if (!ft_strcmp("quit", s) || !ft_strcmp("exit", s))
		{
			safe_send(sock, "quit", 4);
			printf("%s[GOODBYE]%s\n", GREEN, END);
			return ;
		}
		else if (!ft_strcmp("ls", s) || !ft_strcmp("ll", s))
			send_ls(sock);
		else if (!ft_strncmp("get ", s, 4))
			recv_file(sock, s, s + 4, 0);
		else if (!ft_strncmp("put ", s, 4))
			send_file(sock, s, s + 4, 0);
		else
			printf("%sCommand not found [%s]%s\n", LIGHT_RED, s, END);
		ft_putstr(PROMPT);
		free(s);
	}
}

int						main(int ac, char **av)
{
	int					sock;
	struct sockaddr_in	sin;
	struct hostent		*host;

	if (ac != 3)
		return (usage(av[0]));
	signal(SIGCHLD, SIG_IGN);
	sock = init_socket();
	if (!(host = gethostbyname(av[1])))
	{
		fprintf(stderr, "%sCould not resolve hostname: [%s]%s\n",
				RED, av[1], END);
		return (EXIT_FAILURE);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(ft_atoi(av[2]));
	sin.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr));
	ft_memset(&sin.sin_zero, 0, sizeof(sin.sin_zero));
	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) != -1 &&
		printf("%s[CONNECTED]%s\n", GREEN, END))
		handle_client(sock, NULL);
	else
		fprintf(stderr, "%sConnection failed: [%s:%d]%s\n", RED,
				av[1], ft_atoi(av[2]), END);
	return (close(sock));
}
