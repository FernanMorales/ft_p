/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 03:17:43 by fmorales          #+#    #+#             */
/*   Updated: 2014/05/25 21:41:13 by fmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_H
# define SERVEUR_H

# include <stddef.h>

# define END "\033[0;m"
# define RED "\033[0;31m"
# define BLUE "\033[1;34m"
# define GREEN "\033[1;32m"
# define RGREEN "\033[0;32m"
# define LIGHT_RED "\033[1;31m"

int				init_socket(void);
ssize_t			safe_send(int cs, char *buf, size_t len);
void			main_loop(int serv);
size_t			get_file_size(int fd);
void			show_pwd(int cs);
void			show_ls(int cs);
void			change_dir(int cs, char *path);
char			*get_initial_path(int flag);
void			handle_dir(int cs, char *buf);

#endif
