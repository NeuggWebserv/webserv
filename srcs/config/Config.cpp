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
#include "ConfigServer.hpp"
#include "ConfigReader.hpp"
#include "ConfigRequest.hpp"

Config::Config(std::string path)
{
	ConfigServer	server;
	fileVector		file;

	file = ConfigReader::read_file(path.c_str());
	if (file.empty())
	{
		std::cerr << "Could not open default file at location [" << path << "]" << std::endl;
		throw ConfigReader::FileNotFoundException();
	}
	unsigned int	index = 2;
	if (!server.parse_server(index, file)) {
		std::cerr << "Invalid default config file." << std::endl;
		throw ConfigServer::ExceptionInvalidArguments();
	}
	ConfigServer::default_server = server;
	// return server;
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

std::vector<ConfigServer>	Config::get_server_list() const
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
			if (!conf_server.parse_server(i, file))
			{
				std::cerr << "Error: error in config file [" << filename << "]" << std::endl;
				return (1);
			}
			this->server_list.push_back(conf_server);
		}
		else
		{
			std::cerr << "Error: unknown directive [" << file[i] << "]" << std::endl;
			return (1);
		}
	}
	return (0);
}

ConfigRequest	          Config::get_config_for_request(t_listen const address,\
							std::string const uri, std::string host_name, const std::string& method,\
							Request &request) const
{
	ConfigServer	server;
	std::string		location_path;

	host_name = host_name.substr(0, host_name.find_last_of(':'));
	this->get_server_for_request(server, address, host_name);
	server = server.get_location_for_request(uri, location_path);
	if (*(--location_path.end()) == '/')
		location_path.resize(location_path.size() - 1);
	ConfigRequest config(server, request, uri, method, location_path);
	config.set_host_port(address);
	return config;
}

bool		Config::get_server_for_request(ConfigServer &ret, t_listen const address, std::string const host_name) const 
{
	std::vector<ConfigServer>	possible_servers;

	for (std::vector<ConfigServer>::const_iterator servers_iter = this->server_list.begin() ; servers_iter != this->server_list.end(); servers_iter++) 
	{
		std::vector<t_listen>	listens = servers_iter->get_listen();
		for (std::vector<t_listen>::iterator listenIter = listens.begin(); listenIter != listens.end(); listenIter++) 
		{
			if (address.host == (*listenIter).host && address.port == (*listenIter).port) {
				possible_servers.push_back((*servers_iter));
			}
		}
	}
	if (possible_servers.empty())
		return false;
	for (std::vector<ConfigServer>::iterator servers_iter = possible_servers.begin() ; servers_iter != possible_servers.end(); servers_iter++) 
	{
		std::vector<std::string>	server_names = servers_iter->get_server_name();
		for (fileVector::iterator serv_name_iter = server_names.begin() ; serv_name_iter != server_names.end(); serv_name_iter++) 
		{
			if (*serv_name_iter == host_name) 
			{
				ret = *servers_iter;
				return true;
			}
		}
	}
	ret = possible_servers[0];
	return true;
}

std::vector<t_listen>		Config::get_all_listens() const
{
	std::vector<t_listen>	ret;

	for (std::vector<ConfigServer>::const_iterator server = this->server_list.begin(); server != this->server_list.end(); server++) 
	{
		std::vector<t_listen>	listenVec = server->get_listen();
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

std::ostream                        &operator<<(std::ostream &out, const Config &config)
{
	for (size_t i = 0; i < config.get_server_list().size(); i++)
	{
		out << config.get_server_list()[i] << std::endl << std::endl;
	}
	return out;
}