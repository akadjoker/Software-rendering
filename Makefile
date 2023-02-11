# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/20 18:22:02 by lrosa-do          #+#    #+#              #
#    Updated: 2023/02/09 16:56:45 by lrosa-do         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		main

CXX =		g++
CFLAGS =	-g -Wall  -lSDL2 -lm
CFLAGS +=-fsanitize=address

SRCS =		main.cpp

OBJS =		$(SRCS:%.cpp=%.o)

all: $(NAME)

clean:
			$(RM) $(OBJS) 

fclean:		clean
			$(RM) $(NAME)

run:
	@./$(NAME) 
	
	

re:			fclean $(NAME)

.PHONY:		all clean fclean re

$(NAME):	$(SRCS)
	$(CXX)  $(SRCS)  $(CFLAGS) -o $(NAME)
	@./$(NAME) 
