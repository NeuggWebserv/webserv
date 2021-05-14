NAME = a.out

CC = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = test.cpp \
		$(addprefix ../, request_func.cpp Request.cpp) \
		$(addprefix ../../utils/, string.cpp path_is_file.cpp) \
		$(addprefix ../../config/, Config.cpp ConfigReader.cpp ConfigRequest.cpp ConfigServer.cpp ConfigUtil.cpp)

INCL = -I . -I .. -I ../../../includes -I ../../config

all:
	$(CC) $(FLAGS) $(INCL) $(SRCS) -o $(NAME)

clean:
	rm -rf $(NAME)

re: clean all