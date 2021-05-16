#ifndef SERVER_HPP
# define SERVER_HPP

# include "server_socket.hpp"
# include "Config.hpp"
# include "webserv.hpp"

class Server
{
public:
    Server();
    Server(const Server& copy);

    ~Server();

    const Server& operator=(const Server& obj);

    void set_config(const char *filename);
    void set_socket(void);
    void run(void);

private:
    Config                          config;
    std::map<int, ServerSocket>     server_socket_mapping;
    std::map<int, ServerSocket*>    client_socket_mapping;
    std::vector<int>                ready_to_send;
    fd_set                          fd_status;
    size_t                          fd_size;
    int                             max_fd;
};

#endif /* SERVER_HPP */