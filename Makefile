# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpawlows <kpawlows@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/20 05:15:37 by kpawlows          #+#    #+#              #
#    Updated: 2023/04/02 17:53:28 by kpawlows         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror #-fsanitize=thread -g3 #-fsanitize=address
RM			= rm -r

SRCDIR		= src
OBJDIR		= obj
LIBDIR		= lib
LIBFT		= libft.a

SRC			= $(shell find $(SRCDIR) -name '*.c')
OBJ			= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))


$(OBJDIR)/%.o: $(SRCDIR)/%.c
			mkdir -p '$(@D)'
			$(CC) -c $(CFLAGS) $< -o $@

all:		$(NAME)

$(NAME): 	$(OBJ)
			$(CC) $(CFLAGS) -o $@ $^

run:		all
			@./philo 4 410 200 200

clean:
			$(RM) $(OBJDIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY:		all clean fclean re
