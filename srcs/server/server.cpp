#include "server.hpp"

Server::Server()
{
    max_fd = 0;
};

Server::Server(const Server& copy)
{
    *this = copy;
}

Server::~Server()
{
    /* Nothing allocated dynamically */
};

const Server& Server::operator=(const Server& obj)
{
    this->config = obj.config;
    this->server_socket_mapping = obj.server_socket_mapping;
    this->fd_status = obj.fd_status;
    this->fd_size = obj.fd_size;
    this->max_fd = obj.max_fd;

    return *this;
}

void Server::set_config(const char *filename)
{
    config.pull(filename);
}

void Server::set_socket(void)
{
    std::vector<t_listen> all_listens = config.get_all_listens();
    FD_ZERO(&fd_status);
    fd_size = all_listens.size();
    for (std::vector<t_listen>::iterator it = all_listens.begin();
        it != all_listens.end(); ++it)
    {
        ServerSocket server_socket(*it);
        int fd;
        server_socket.setup();
        fd = server_socket.get_fd();
        FD_SET(fd, &fd_status);
        server_socket_mapping.insert(std::make_pair(fd, server_socket));
        if (fd > max_fd)
            max_fd = fd;
    }
    if (max_fd == 0)
        throw std::logic_error("Error set_socket: No listen");
}

void Server::run(void)
{
    fd_set reading_status;
    fd_set writing_status;
    struct timeval timeout;
    int ret;

    while (1)
    {
        ret = 0;
        while (ret == 0)
        {
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            memcpy(&reading_status, &fd_status, sizeof(fd_status));
            FD_ZERO(&writing_status);
            for (std::vector<int>::const_iterator it = ready_to_send.begin();
                it != ready_to_send.end(); ++it)
                FD_SET(*it, &writing_status);
            std::cout << "Waiting connection\n";
            ret = select(max_fd + 1, &reading_status, &writing_status, NULL, &timeout);
        }
        if (ret > 0)
        {
            for (std::vector<int>::iterator it = ready_to_send.begin();
                it != ready_to_send.end(); ++it)
            {
                if (FD_ISSET(*it, &writing_status))
                {
                    ret = client_socket_mapping[*it]->send(*it);
                    if (ret == 0)
                    {
                        ready_to_send.erase(it);
                    }
                    else if (ret < 0)
                    {
                        FD_CLR(*it, &fd_status);
                        ready_to_send.erase(it);
                        client_socket_mapping.erase(*it);
                    }
                    ret = 0;
                    break ;
                }
            }
            if (ret == 0)
                continue ;

            int client_fd;
            for (std::map<int, ServerSocket*>::iterator it = client_socket_mapping.begin();
                it != client_socket_mapping.end(); ++it)
            {
                client_fd = it->first;
                if (FD_ISSET(client_fd, &reading_status))
                {
                    ret = it->second->recv(client_fd);
                    if (ret == 0)
                    {
                        it->second->do_request(client_fd, config);
                        ready_to_send.push_back(client_fd);
                    }
                    else if (ret < 0)
                    {
                        FD_CLR(client_fd, &fd_status);
                        client_socket_mapping.erase(client_fd);
                    }
                    ret = 0;
                    break ;
                }
            }
            if (ret == 0)
                continue ;

            for (std::map<int, ServerSocket>::iterator it = server_socket_mapping.begin();
                it != server_socket_mapping.end(); ++it)
            {
                if (FD_ISSET(it->first, &reading_status))
                {
                    if ((client_fd = it->second.accept()) >= 0)
                    {
                        FD_SET(client_fd, &fd_status);
                        client_socket_mapping.insert(std::make_pair(client_fd, &it->second));
                        if (client_fd > max_fd)
                            max_fd = client_fd;
                    }
                    break ;
                }
            }
        }
        else
        {
            std::cerr << "ERROR run: problem with select\n";
            for (std::map<int, ServerSocket*>::iterator it = client_socket_mapping.begin();
                it != client_socket_mapping.end(); ++it)
            {
                it->second->close_client_fd(it->first);
            }
            ready_to_send.clear();
            client_socket_mapping.clear();
            FD_ZERO(&fd_status);
            for (std::map<int, ServerSocket>::const_iterator it = server_socket_mapping.begin();
                it != server_socket_mapping.end(); ++it)
            {
                FD_SET(it->first, &fd_status);
            }
        }
    }
}