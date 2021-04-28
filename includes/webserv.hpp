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
# include <set>

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

/*
 * typedef part
 */
typedef struct  s_listen
{
    unsigned    host;
    int         port;
}               t_listen;

typedef struct  s_error_page {
	std::vector<int>    error_codes; // all of the codes that will be redirected
	std::string         uri;		// uri to which they are redirected
}               t_error_page;

typedef std::vector<std::string> fileVector;

#endif /* WEBSERV_HPP */
