CC = c++
FLAGS = -Wall -Wextra -Werror -MMD -MP -g3 -std=c++98

COMMAND_DIR = Commands

SRCS =	main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		Bot.cpp \
		Commands/CommandController.cpp \
		Commands/Command.cpp \
		Commands/NICK.cpp \
		Commands/USER.cpp \
		Commands/PRIVMSG.cpp \
		Commands/JOIN.cpp \
		Commands/CAP.cpp \
		Commands/UNKNOWN.cpp \
		Commands/MODE.cpp \
		Commands/PASS.cpp \
		Commands/TOPIC.cpp \
		Commands/KICK.cpp \
		Commands/INVITE.cpp \
		Commands/PART.cpp \
		Commands/QUIT.cpp 

OBJS = $(SRCS:.cpp=.o)
NAME = ircserv

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
	$(CC) $(FLAGS) -c $< -o $@ -I. -ICommands

clean :
	rm -rf $(OBJS)
	rm -rf $(SRCS:.cpp=.d)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean re fclean