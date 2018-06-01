##
## EPITECH PROJECT, 2018
## 
## File description:
## 
##

NAME	= plazza

CC	= g++

RM	= rm -f

SRCS	= ./src/main.cc \


OBJS	= $(SRCS:.cc=.o)

CXXFLAGS = -Iinc
CXXFLAGS += -W -Wall -Wextra
CXXFLAGS += -std=c++14

LIB = -pthread

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(CXXFLAGS) $(LIB)

clean:
	$(RM) $(OBJS)
	$(RM) src/*~
	$(RM) inc/*~
	$(RM) ./*~

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
