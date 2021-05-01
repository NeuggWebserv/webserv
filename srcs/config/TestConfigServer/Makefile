NAME = configServer

CC = clang++

FLAGS = -std=c++98 -Wall -Wextra -Werror

SRCS = test.cpp \
		../ConfigServer.cpp \
		../ConfigUtil.cpp

INCL = -I ../../../includes -I ../

$(NAME) :
	$(CC) $(FLAGS) $(INCL) $(SRCS) -o $(NAME)

all : $(NAME)

clean :
	rm -rf $(NAME)

re : clean $(NAME)