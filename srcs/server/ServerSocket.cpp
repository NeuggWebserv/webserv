#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const t_listen& lstn) : lstn(lstn)
{
    server_fd = -1;
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(lstn.host);
    address.sin_port = htons(lstn.port);
}

ServerSocket::ServerSocket(const ServerSocket& copy)
{
    *this = copy;
}

ServerSocket::~ServerSocket()
{
    /* Nothing allocated dynamically */
}

const ServerSocket& ServerSocket::operator=(const ServerSocket& obj)
{
    this->server_fd = obj.server_fd;
    this->lstn = obj.lstn;
    this->address = obj.address;

    return *this;
}

void ServerSocket::setup()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw std::logic_error("Error setup: Cannot create socket");
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
    client_msg_mapping.insert(std::make_pair(client_fd, ""));
    return client_fd;
}

int ServerSocket::send(int client_fd)
{
    if (sent.find(client_fd) == sent.end()) // initiate sending
        sent.insert(std::make_pair(client_fd, 0));
    std::string partial_str = client_msg_mapping[client_fd].substr(sent[client_fd], PACKET_SIZE);

    int ret = ::send(client_fd, partial_str.c_str(), partial_str.size(), 0);
    if (ret < 0)
    {
        close_client_fd(client_fd);
        sent.erase(client_fd);
        return -1;
    }
    sent[client_fd] += ret;
    if (sent[client_fd] >= client_msg_mapping[client_fd].size())
    {
        client_msg_mapping.erase(client_fd);
        sent.erase(client_fd);
        return 0;
    }
    return 1;
}

int ServerSocket::recv(int client_fd)
{
    char recv_buffer[PACKET_SIZE] = {0};
    int ret;
	
	ret = ::recv(client_fd, recv_buffer, PACKET_SIZE - 1, 0);
    if (ret > 0)
    {
        client_msg_mapping[client_fd] += std::string(recv_buffer);

        size_t end_of_header = client_msg_mapping[client_fd].find("\r\n\r\n");
        if (end_of_header != std::string::npos)
        {
            if (client_msg_mapping[client_fd].find("Content-Length: ") == std::string::npos)
            {
                if (client_msg_mapping[client_fd].find("Transfer-Encoding: chunked") == std::string::npos)
                    return 0;
                if (is_suffix(client_msg_mapping[client_fd], "0\r\n\r\n"))
                    return 0;
                return 1;
            }
            size_t len = std::atoi(client_msg_mapping[client_fd].substr(
                client_msg_mapping[client_fd].find("Content-Length: ") + 16, 10).c_str()); // substr 10 characters expecting it's smaller or equal than 10 digits
            if (client_msg_mapping[client_fd].size() >= end_of_header + len + 4) // add 4 because of "\r\n\r\n"
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
	ConfigRequest	request_conf;
	Response		response;
	std::string		recvd = "";

	if (client_msg_mapping[client_fd].find("Transfer-Encoding: chunked") != std::string::npos &&
		client_msg_mapping[client_fd].find("Transfer-Encoding: chunked") < client_msg_mapping[client_fd].find("\r\n\r\n"))
		this->process_chunk(client_fd);
	if (client_msg_mapping[client_fd] != "")
	{
		Request			request(client_msg_mapping[client_fd]);

		if (request.get_ret() != 200)
			request.set_method("GET");

		request_conf = config.get_config_for_request(this->lstn,  request.get_path(), request.get_headers().at("Host"), request.get_method(), request);

		response.call(request, request_conf);

		client_msg_mapping.erase(client_fd);
		client_msg_mapping.insert(std::make_pair(client_fd, response.get_response()));
	}
}

void		ServerSocket::process_chunk(int client_fd)
{
	std::string	head = client_msg_mapping[client_fd].substr(0, client_msg_mapping[client_fd].find("\r\n\r\n"));
	std::string	chunks = client_msg_mapping[client_fd].substr(client_msg_mapping[client_fd].find("\r\n\r\n") + 4, client_msg_mapping[client_fd].size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}

	client_msg_mapping[client_fd] = head + "\r\n\r\n" + body + "\r\n\r\n";
}

void ServerSocket::close_client_fd(int client_fd)
{
    if (client_fd < 0)
        return ;
    close(client_fd);
    client_msg_mapping.erase(client_fd);
    sent.erase(client_fd);
}

int ServerSocket::get_fd() const
{
    return server_fd;
}

ServerSocket::ServerSocket()
{
    /* forbidden */
}
