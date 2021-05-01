NAME = webserv

CC = clang++

CFLAGS = -D TEST -std=c++98

INCLUDES = -I includes -I srcs/config -I srcs/server -I srcs/utils

# FILES

MAIN = webserv

CONFIG = Config ConfigReader ConfigServer ConfigUtil

SERVER = server server_socket

UTILS = pathIsFile string_utils

# SRCS & OBJS

SRC_DIR = srcs

OBJ_DIR = objs

OBJ_SUBDIR = $(addprefix $(OBJ_DIR)/, config server utils)

SRCS = $(addsuffix .cpp, $(MAIN)) \
	   $(addprefix config/, $(addsuffix .cpp, $(CONFIG))) \
	   $(addprefix server/, $(addsuffix .cpp, $(SERVER))) \
	   $(addprefix utils/, $(addsuffix .cpp, $(UTILS)))

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
