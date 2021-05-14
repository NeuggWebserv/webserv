/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 16:17:48 by youlee            #+#    #+#             */
/*   Updated: 2021/04/28 16:17:49 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigRequest.hpp"
#include "ConfigServer.hpp"

ConfigRequest::ConfigRequest(void) {}
ConfigRequest::~ConfigRequest(void) {}
ConfigRequest::ConfigRequest(ConfigServer &config, Request &request, const std::string &path,
							const std::string &method, std::string location_name)
: error_page(config.get_error_page()), cli_body_size(config.get_client_body_buffer_size())
, cgi_param(config.get_cgi_param()), cgi_pass(config.get_cgi_pass())
, allowed_method(config.get_allowed_methods()), len("")
, auto_index(config.get_autoindex())
{
	std::string				alias = config.get_alias();
	std::string				root = config.get_root();
	std::string				ret;
	std::vector<std::string>	conf_index = config.get_index();

	//Process index contents inside config file
	for(std::vector<std::string>::const_iterator it = conf_index.begin();it != conf_index.end();it++)
	{
		std::vector<std::string>::const_iterator it2 = index.begin();
		for(it2 = index.begin();it2 != index.end(); it2++)
			if (*it == *it2) break ;
		if (it2 == index.end())
			index.push_back(*it);	
	}
	cgi_param = request.get_env();
	if (location_name[0] != '*' && config.get_alias_set())
	{
		ret = root + alias + path.substr(location_name.length());
		this->content_location = alias + remove_adjacent_slashes(path.substr(location_name.length()));
	}
	else
	{
		ret = root + path;
		this->content_location = remove_adjacent_slashes(path);
	}
	this->path = remove_adjacent_slashes(ret);
	std::string index_path;
	if (!path_is_file(this->path) && method == "GET")
	{
		if ((index_path = this->add_index(request)) != "")
		{
			config = config.get_location_for_request(index_path, location_name);
			this->cgi_pass = config.get_cgi_pass();
			this->cgi_param = config.get_cgi_param();
		}
	}
}

ConfigRequest::ConfigRequest(ConfigRequest const &src)
{
	if (this != &src)
	{
		this->content_location = src.content_location;
		this->path = src.path;
		this->error_page = src.error_page;
		this->cli_body_buf_size = src.cli_body_buf_size;
		this->cgi_param = src.cgi_param;
		this->cgi_pass = src.cgi_pass;
		this->allowed_method = src.allowed_method;
		this->auto_index = src.auto_index;
		this->index = src.index;
	}
	return ;
}

ConfigRequest &ConfigRequest::operator=(ConfigRequest const &src)
{
	if (this != &src)
	{
		this->content_location = src.content_location;
		this->path = src.path;
		this->error_page = src.error_page;
		this->cli_body_size = src.cli_body_size;
		this->cgi_param = src.cgi_param;
		this->cgi_pass = src.cgi_pass;
		this->allowed_method = src.allowed_method;
		this->auto_index = src.auto_index;
		this->index = src.index;
		this->host_port.host = src.host_port.host;
		this->host_port.port = src.host_port.port;
	}
	return (*this);
}

// Getter Function

const std::string &ConfigRequest::get_content_location() const
{
	return (this->content_location);
}
const std::string							&ConfigRequest::get_path() const
{
	return (this->path);
}
const std::map<int, std::string>			&ConfigRequest::get_error_page() const
{
	return (this->error_page);
}
const unsigned long							&ConfigRequest::get_cli_body_size() const
{
	return (this->cli_body_size);
}
const std::map<std::string, std::string>	&ConfigRequest::get_cgi_param() const
{
	return (this->cgi_param);
}
const std::string							&ConfigRequest::get_cgi_pass() const
{
	return (this->cgi_pass);
}
const std::set<std::string>					&ConfigRequest::get_allowed_method() const
{
	return (this->allowed_method);
}
const t_listen								&ConfigRequest::get_host_port() const
{
	return (this->host_port);
}
const std::string							&ConfigRequest::get_len() const
{
	return (this->lang);
}
const std::vector<std::string>				&ConfigRequest::get_index() const
{
	return (this->index);
}
const bool									&ConfigRequest::get_auto_index() const
{
	return (this->auto_index);
}

//setter function
void										ConfigRequest::set_path(int code)
{
	//default path update process
	this->path = "./files/error/";
	this->path += to_string(code) + ".html";
}
void										ConfigRequest::set_path(const std::string&)
{
	this->path = path;
}
void										ConfigRequest::set_content_location(const std::string&)
{
	this->content_location = path;
}
void										ConfigRequest::set_host_port(const t_listen hostport)
{
	this->host_port.port = host_port.port;
	this->host_port.host = host_port.host;
}


//other function
std::string									ConfigRequest::add_index(Request &req)
{
	std::vector<std::string>::iterator							it;
	std::list<std::pair<std::string, float> >::const_iterator	lang;
	std::string													path;

	it = this->index.begin();
	while (it != this->index.end())
	{
		for(lang = req.get_lang().begin(); lang != req.get_lang().end(); lang++)
		{
			path = this->path;
			if (path[path.size() - 1] != '/')
				path += "/";
			if ((*it).find('.') != (*it).npos)
				path += (*it).substr(0, (*it).find_last_of('.') + 1) + lang->first + (*it).substr((*it).find_last_of('.'));
			if (path_is_file(path))
			{
				this->path = path;
				if (this->content_location.size() && this->content_location[this->content_location.size() - 1] != '/')
					this->content_location += "/";
				if ((*it).find('.') != (*it).npos)
					this->content_location += (*it).substr(0, (*it).find_last_of('.') + 1) + lang->first + (*it).substr((*it).find_last_of('.'));
				return (this->path);
			}
		}
		it++;
	}
	
	it = this->index.begin();
	while (it != this->index.end())
	{
		path = this->path;
		if (path[path.size() - 1] != '/')
			path += "/";
		path += *it;
		if (path_is_file(path))
		{
			this->path = path;
			if (this->content_location[this->content_location.size() - 1] != '/')
				this->content_location += "/";
			this->content_location += *it;
			return (this->path);
		}
		it++;
	}
	return ("");
}

//operator<< function

std::ostream &operator<<(std::ostream &out, ConfigRequest &req)
{
	out << "path: " << req.path << std::endl;
	out << "content_location: " << req.content_location << std::endl;
	out << "error_page:" << std::endl;
	for (std::map<int, std::string>::iterator i = req.error_page.begin(); i != req.error_page.end(); i++) {
		out << "\t" << i->first << " " << i->second << std::endl;
	}
	out << "client_body_buffer_size: " << req.cli_body_size << std::endl;
	out << "cgi_param:" << std::endl;
	for (std::map<std::string, std::string>::iterator i = req.cgi_param.begin() ; i != req.cgi_param.end(); i++)
		out << "\t" << i->first << "=" << i->second << std::endl;
	out << "cgi_pass: " << req.cgi_pass << std::endl;
	out << "allowed_methods:" << std::endl;
	for (std::set<std::string>::iterator i = req.allowed_method.begin(); i != req.allowed_method.end(); i++)
		out << "\t" << *i << std::endl;
	out << "index:" << std::endl;
	for (std::vector<std::string>::iterator i = req.index.begin(); i != req.index.end(); i++)
		out << "\t" << *i << std::endl;
	out << "autoindex: " << (req.auto_index ? "on" : "off") << std::endl;
	return out;
}