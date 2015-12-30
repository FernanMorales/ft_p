/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 03:17:43 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/18 23:12:39 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <stddef.h>

# define END "\033[0;m"
# define RED "\033[0;31m"
# define BLUE "\033[1;34m"
# define GREEN "\033[1;32m"
# define RGREEN "\033[0;32m"
# define LIGHT_RED "\033[1;31m"
# define PROMPT "\033[1;37mftp> \033[0;m"

int				init_socket(void);
ssize_t			safe_send(int cs, char *buf, size_t len);
ssize_t			safe_recv(int cs, char *buf, size_t len);
size_t			get_file_size(int fd);
void			send_cd(int sock, char *s);
void			display(uint32_t tot, uint32_t rest);
void			send_pwd(int sock);
void			send_ls(int sock);
struct hostent	*get_hostname(char *hostname);
void			show_pwd(void);
int				check_client_commands(int sock, char *s);

#endif
