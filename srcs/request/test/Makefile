NAME = a.out

CC = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = test.cpp \
		$(addprefix ../, request_func.cpp Request.cpp) \
		../../utils/string.cpp

INCL = -I . -I .. -I ../../../includes

all:
	$(CC) $(FLAGS) $(INCL) $(SRCS) -o $(NAME)

clean:
	rm -rf $(NAME)

re: clean all