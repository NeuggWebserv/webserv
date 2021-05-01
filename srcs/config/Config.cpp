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

Config	&Config::operator=(Config const &Co)
{
	if (this == &Co)
		return (*this);
	this->server_list = Co.server_list;
	return (*this);
}

Config::~Config() { return ;}

std::vector<ConfigServer>	Config::getServer_list() const
{
	return (this->server_list);
}

int		Config::pull(const char *filename)
{
	fileVector			file;
	unsigned int			file_size;

	file = ConfigReader::readFile(filename);
	file_size = file.size();
	for (unsigned int i = 0; i < file_size; i++)
	{
		if (file[i] == "server")
		{
			ConfigServer conf_server;
			++i;
			if (file[i] != "{")
			{
				// std::cerr << RED << "Error: expected '{' after server directive." << RESET << std::endl;
				return (1);
			}
			++i;
			if (!conf_server.parse_server(i, file))
			{
				// std::cerr << RED << "Error: error in config file [" << filename << "]" << RESET <<  std::endl;
				return (1);
			}
			this->server_list.push_back(conf_server);
		}
		else
		{
			// std::cerr << RED << "Error: unknown directive [" << file[i] << "]" << RESET << std::endl;
			return (1);
		}
	}
	return (0);
}

std::vector<t_listen>				Config::get_all_listens() const {
	std::vector<t_listen>	ret;

	for (std::vector<ConfigServer>::const_iterator server = this->server_list.begin(); server != this->server_list.end(); server++) {
		std::vector<t_listen>	listenVec = server->get_listen();
		for (std::vector<t_listen>::iterator listen = listenVec.begin(); listen != listenVec.end(); listen++) {
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