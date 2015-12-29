#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmorales <fmorales@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/19 14:45:43 by fmorales          #+#    #+#              #
#    Updated: 2014/05/18 13:34:48 by fmorales         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME_CLIENT = client
NAME_SERVEUR = serveur

SRC_CLIENT = clien/main.c \
			clien/builtin.c \
			clien/socket_common.c

SRC_SERVEUR = server/main.c \
			server/builtin.c \
			server/socket_common.c

OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVEUR = $(SRC_SERVEUR:.c=.o)

all: $(NAME_CLIENT) $(NAME_SERVEUR)

$(NAME_CLIENT): $(OBJ_CLIENT)
	make -C libft/
	clang -O3 -Llibft -lft -o $@ $^

$(NAME_SERVEUR): $(OBJ_SERVEUR)
	make -C libft/
	clang -O3 -Llibft -lft -o $@ $^

%.o: %.c
	clang -O3 -Wall -Wextra -Werror -Ilibft -o $@ -c $^

clean:
	make -C libft/ clean
	rm -f $(OBJ_CLIENT) $(OBJ_SERVEUR)

fclean: clean
	make -C libft/ fclean
	rm -f $(NAME_CLIENT) $(NAME_SERVEUR)

re: fclean all

.PHONY: all clean fclean re
