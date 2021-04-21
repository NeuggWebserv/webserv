#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// CPP Includes
# include <iostream>
# include <iomanip>
# include <string>
# include <limits>

// CPP Containers
# include <algorithm>
# include <utility>
# include <map>
# include <vector>

// C Includes
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <time.h>
# include <errno.h>
# include <dirent.h>
# include <string.h>

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

// Defines
#ifndef PACKET_SIZE
# define PACKET_SIZE 1400
#endif

typedef struct  s_listen
{
    unsigned    host;
    int         port;
}               t_listen;

// string_utils
bool is_suffix(const std::string& src, const std::string& suffix);

#endif /* WEBSERV_HPP */