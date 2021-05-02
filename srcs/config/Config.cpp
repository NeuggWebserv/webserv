/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 19:05:58 by youlee            #+#    #+#             */
/*   Updated: 2021/04/21 19:06:02 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(std::string Path)
{
	//Reader doesn't exist yet.
}

Config::Config(Config const &src)
{
	if (this == &src)
		return ;
	this->server_list = src.server_list;
}

Config	&Config::operator=(Config const &co)
{
	if (this == &co)
		return (*this);
	this->server_list = co.server_list;
	return (*this);
}

Config::~Config() { return ;}

std::vector<ConfigServer>	Config::getServer_list() const
{
	return (this->server_list);
}

int		Config::pull(const char *filename)
{
	fileVector				file;
	unsigned int			file_size;

	file = ConfigReader::read_file(filename);
	file_size = file.size();
	for (unsigned int i = 0; i < file_size; i++)
	{
		if (file[i] == "server")
		{
			ConfigServer conf_server;
			++i;
			if (file[i] != "{")
			{
				std::cerr << "Error: expected '{' after server directive." << std::endl;
				return (1);
			}
			++i;
			if (!conf_server.parseServer(i, file))
			{
				std::cerr << "Error: error in config file [" << filename << "]" << std::endl;
				return (1);
			}
			this->server_list.push_back(server);
		}
		else
		{
			std::cerr << "Error: unknown directive [" << file[i] << "]" << std::endl;
			return (1);
		}
	}
	return (0);
}

bool		Config::getServer_for_request(ConfigServer &ret, t_listen const address, std::string const host_name) const 
{
	std::vector<ConfigServer>	possibleServers;

	for (std::vector<ConfigServer>::const_iterator serversIter = this->_servers.begin() ; serversIter != this->_servers.end(); serversIter++) 
	{
		std::vector<t_listen>	listens = serversIter->getListen();
		for (std::vector<t_listen>::iterator listenIter = listens.begin(); listenIter != listens.end(); listenIter++) 
		{
			if (address.host == (*listenIter).host && address.port == (*listenIter).port) {
				possibleServers.push_back((*serversIter));
			}
		}
	}
	if (possibleServers.empty())
		return false;
	for (std::vector<ConfigServer>::iterator serversIter = possibleServers.begin() ; serversIter != possibleServers.end(); serversIter++) 
	{
		std::vector<std::string>	serverNames = serversIter->getServerName();
		for (fileVector::iterator servNameIter = serverNames.begin() ; servNameIter != serverNames.end(); servNameIter++) 
		{
			if (*servNameIter == hostName) 
			{
				ret = *serversIter;
				return true;
			}
		}
	}
	ret = possibleServers[0];
	return true;
}

std::vector<t_listen>		getAllListens() const
{
	std::vector<t_listen>	ret;

	for (std::vector<ConfigServer>::const_iterator server = this->_servers.begin(); server != this->_servers.end(); server++) 
	{
		std::vector<t_listen>	listenVec = server->getListen();
		for (std::vector<t_listen>::iterator listen = listenVec.begin(); listen != listenVec.end(); listen++) 
		{
			std::vector<t_listen>::iterator i = ret.begin();
			for ( ; i != ret.end(); i++)
				if (listen->host == i->host && listen->port == i->port)
					break ;
			if (i == ret.end())
				ret.push_back(*listen);
		}
	}
	return ret;
}