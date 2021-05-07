#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// CPP Includes
# include <iostream>
# include <iomanip>
# include <string>
# include <limits>
# include <sstream>

// CPP Containers
# include <algorithm>
# include <utility>
# include <map>
# include <vector>
# include <set>
# include <list>

// C Includes
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <time.h>
# include <errno.h>
# include <dirent.h>

// C system
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>

// C Network
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>

typedef struct  s_listen
{
    unsigned    host;
    int         port;
}               t_listen;

// srcs/utils function

std::string					read_key(char *line);
std::string					read_key(const std::string& line);
std::string					read_value(char *line);
std::string					read_value(const std::string& line);
std::string&				strip(std::string& str, char c);
std::string&				capitalize(std::string& str);
std::string					to_string(size_t n);
std::string&				str_to_lower(std::string& str);
std::string&				str_to_upper(std::string& str);
std::string&				pop_str_tail(std::string& str);
std::vector<std::string>	split(const std::string &str, char c);

//				Compare
bool						compare_langs(const std::pair<std::string, float> first, const std::pair<std::string, float> second);
#endif /* WEBSERV_HPP */