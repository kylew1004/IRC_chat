# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/06 13:01:40 by klew              #+#    #+#              #
#    Updated: 2023/06/13 17:11:15 by klew             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic -g
FLAGS_S	=	-g -fsanitize=address
FLAGS_OPT = -O3

SRCS	=	srcs/utils.cpp \
		srcs/SockServer.cpp \
		srcs/main.cpp \
		srcs/User.cpp \
		srcs/socketConf.cpp \
		srcs/SockAddress.cpp \
		srcs/Channels.cpp \
		srcs/Commandes/Server/INFO.cpp \
		srcs/Commandes/Server/TIME.cpp \
		srcs/Commandes/Users/PASS.cpp \
		srcs/Commandes/Users/QUIT.cpp \
		srcs/Commandes/Users/USER.cpp \
		srcs/Commandes/Users/NICK.cpp \
		srcs/Commandes/Users/OPER.cpp \
		srcs/Commandes/Requests/WHO.cpp \
		srcs/Commandes/Messages/PRIVMSG.cpp \
		srcs/Commandes/Channel/KICK.cpp \
		srcs/Commandes/Channel/TOPIC.cpp \
		srcs/Commandes/Channel/JOIN.cpp \
		srcs/Commandes/Channel/MODE.cpp \
		srcs/Commandes/Channel/INVITE.cpp \
		srcs/Commandes/Channel/LIST.cpp \
		srcs/Commandes/Channel/NAMES.cpp \
		srcs/Commandes/Channel/PART.cpp \
		srcs/Commandes/Other/ERROR.cpp


OBJS	=	$(SRCS:.cpp=.o)

all: $(NAME) 

$(NAME): $(OBJS)
			$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re:	fclean all


%.o:	%.cpp
		$(CC) $(FLAGS) -o $@ -c $<

.PHONY: all clean fclean re
