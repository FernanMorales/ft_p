/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_common.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 03:19:04 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 20:24:00 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

int						init_socket(void)
{
	int					sock;
	struct protoent		*proto;

	proto = getprotobyname("tcp");
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	return (sock);
}

ssize_t					safe_send(int cs, char *buf, size_t len)
{
	ssize_t				sz;
	ssize_t				sent;

	sent = 0;
	while (len > 0 && (sz = write(cs, buf + sent, len)) > 0)
	{
		sent += sz;
		len -= sz;
	}
	return (sent);
}

ssize_t					safe_recv(int cs, char *buf, size_t len)
{
	ssize_t				sz;
	ssize_t				sent;

	sent = 0;
	while (len > 0 && (sz = read(cs, buf + sent, len)) > 0)
	{
		sent += sz;
		len -= sz;
	}
	return (sent);
}
