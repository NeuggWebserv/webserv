NAME = webserv

CC = clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = -I includes -I srcs/config -I srcs/server

# FILES

MAIN = webserv

CONFIG = config

SERVER = server server_socket

# SRCS & OBJS

SRC_DIR = srcs

OBJ_DIR = objs

OBJ_SUBDIR = $(addsuffix $(OBJ_DIR)/, config server)

SRCS = $(addsuffix .cpp, $(MAIN)) \
	   $(addprefix config/, $(addsuffix, .cpp, $(CONFIG))) \
	   $(addprefix server/, $(addsuffix, .cpp, $(SERVER)))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# MAKE

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_SUBDIR)

all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
