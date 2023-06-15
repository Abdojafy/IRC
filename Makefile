# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: junik <abderrachidyassir@gmail.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/14 12:11:22 by ayassir           #+#    #+#              #
#    Updated: 2023/06/15 11:56:13 by junik            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CC			= c++

STD			= -std=c++98

SRC			= main.cpp ircserv.cpp

INC			= ircserv.hpp 

OBJ			= $(SRC:.cpp=.o) 

CXXFLAGS	= -Wall -Wextra -Werror

all : $(NAME) 

$(NAME) : $(OBJ)
	$(CC) $(CXXFLAGS) $(STD) $(OBJ) -o $(NAME) 

%.o : %.cpp $(INC)
	$(CC) $(CXXFLAGS) -c $< -o $@ 

clean :
	rm -rf $(OBJ) 

fclean : clean
	rm -rf $(NAME) 

re : fclean all 