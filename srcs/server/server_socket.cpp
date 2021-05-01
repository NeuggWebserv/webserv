#include "server_socket.hpp"

ServerSocket::ServerSocket(t_listen* lstn) : lstn(lstn) {}

ServerSocket::~ServerSocket()
{

}

void ServerSocket::setup()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw std::logic_error("Error setup: Cannot create socket");
    address.sin_family = AF_INET;

    printf("test address: %u , %d", lstn->host, lstn->port);

    address.sin_addr.s_addr = htonl(lstn->host);
    address.sin_port = htons(lstn->port);
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::logic_error("Error setup: Cannot bind");
    if (listen(server_fd, 1000) < 0)
        throw std::logic_error("Error setup: Cannot listen");
}

int ServerSocket::accept()
{
    int client_fd = ::accept(server_fd, NULL, NULL);

    if (client_fd < 0)
    {
        std::cerr << "Error accept: Cannot accept\n";
        return -1;
    }
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    client_request_mapping.insert(std::make_pair(client_fd, ""));
    return client_fd;
}

int ServerSocket::send(int client_fd)
{
    static std::map<int, size_t> sent;

    if (sent.find(client_fd) == sent.end())
        sent.insert(std::make_pair(client_fd, 0));
    std::string partial_str = client_request_mapping[client_fd].substr(sent[client_fd], PACKET_SIZE);

    int ret = ::send(client_fd, partial_str.c_str(), partial_str.size(), 0);
    if (ret < 0)
    {
        close_client_fd(client_fd);
        sent[client_fd] = 0;
        return -1;
    }
    sent[client_fd] += ret;
    if (sent[client_fd] >= client_request_mapping[client_fd].size())
    {
        client_request_mapping.erase(client_fd);
        sent[client_fd] = 0;
        return 0;
    }
    return 1;
}

int ServerSocket::recv(int client_fd)
{
    int ret = ::recv(client_fd, recv_buffer, PACKET_SIZE - 1, 0);

    if (ret > 0)
    {
        client_request_mapping[client_fd] += std::string(recv_buffer);

        size_t end_of_header = client_request_mapping[client_fd].find("\r\n\r\n");
        if (end_of_header != std::string::npos)
        {
            if (client_request_mapping[client_fd].find("Content-Length: ") == std::string::npos)
            {
                if (client_request_mapping[client_fd].find("Transfer-Encoding: chunked") == std::string::npos)
                    return 0;
                if (is_suffix(client_request_mapping[client_fd], "0\r\n\r\n"))
                    return 0;
                return 1;
            }
            size_t len = std::atoi(client_request_mapping[client_fd].substr(
                client_request_mapping[client_fd].find("Content-Length: ") + 16, 10).c_str());
            if (client_request_mapping[client_fd].size() >= end_of_header + len + 4)
                return 0;
            else
                return 1;
        }
        return 1;
    }
    else if (ret == 0)
    {
        std::cerr << "Connection closed by client\n";
    }
    else
    {
        std::cerr << "Error recv: error on receiving request from cliend_fd\n";
    }
    close_client_fd(client_fd);
    return -1;
}

void ServerSocket::do_request(int client_fd, const Config& config)
{
    /* For test */
    client_request_mapping[client_fd] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
}

void ServerSocket::close_client_fd(int client_fd)
{
    if (client_fd < 0)
        return ;
    close(client_fd);
    client_request_mapping.erase(client_fd);
}

int ServerSocket::get_fd() const
{
    return server_fd;
}

ServerSocket::ServerSocket()
{
    /* forbidden */
}