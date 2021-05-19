#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include "Config.hpp"
# include "ConfigRequest.hpp"
# include "Response.hpp"
# include "webserv.hpp"

class Config;

class ServerSocket
{
public:
    ServerSocket(const t_listen& lstn);
    ServerSocket(const ServerSocket& copy);
    ~ServerSocket();

    const ServerSocket& operator=(const ServerSocket& obj);

    void setup();
    int accept(void);
    int send(int client_fd);
    int recv(int client_fd);
    void do_request(int client_fd, const Config& config);
    void close_client_fd(int client_fd);
    int get_fd() const;
    void process_chunk(int client_fd);

private:
    ServerSocket();

    int                         server_fd;
    t_listen                    lstn;
    struct sockaddr_in          address;
    std::map<int, std::string>  client_msg_mapping;
    std::map<int, size_t>       sent; // mapping client_fd and idx of message sent
};

#endif /* SERVER_SOCKET_HPP */
