NAME = webserv

CC = clang++

CFLAGS = -std=c++98

INCLUDES = -I includes -I srcs/config -I srcs/server -I srcs/response -I srcs/request -I srcs/cgi -I srcs/autoindex

# FILES

MAIN = webserv

CONFIG = Config ConfigReader ConfigRequest ConfigServer ConfigUtil

SERVER = server server_socket

RESPONSE = Response response_header

REQUEST = Request request_func

CGI = Cgi

AUTOINDEX = AutoIndex

UTILS = string_utils string path_is_file

# SRCS & OBJS

SRC_DIR = srcs

OBJ_DIR = objs

OBJ_SUBDIR = $(addprefix $(OBJ_DIR)/, config server utils response request cgi autoindex)

SRCS = $(addsuffix .cpp, $(MAIN)) \
	   $(addprefix config/, $(addsuffix .cpp, $(CONFIG))) \
	   $(addprefix server/, $(addsuffix .cpp, $(SERVER))) \
	   $(addprefix response/, $(addsuffix .cpp, $(RESPONSE))) \
	   $(addprefix request/, $(addsuffix .cpp, $(REQUEST))) \
	   $(addprefix cgi/, $(addsuffix .cpp, $(CGI))) \
	   $(addprefix autoindex/, $(addsuffix .cpp, $(AUTOINDEX))) \
	   $(addprefix utils/, $(addsuffix .cpp, $(UTILS)))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# MAKE

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
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

