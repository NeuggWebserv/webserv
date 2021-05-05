NAME = config

CC = clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

INCL = -I ../../../includes -I ../

SRCS = test.cpp \
		$(addprefix ../, Config.cpp ConfigReader.cpp ConfigUtil.cpp ConfigServer.cpp)

$(NAME) :
	$(CC) $(CFLAGS) $(INCL) -o $(NAME) $(SRCS)

all: $(NAME)

clean:
	rm -f $(NAME)

re: clean all
