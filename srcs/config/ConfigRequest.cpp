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
	cgi_param = request.getEnv();
	if (location_name[0] != '*' && config.get_alias_set())
	{
		ret = root + alias + path.substr(location_name.length());
		this->content_location = alias + remove_adjacent_slashes(path.substr(location_name,length()));
	}
	else
	{
		ret = root + path;
		this->content_location = remove_adjacent_slashes(path);
	}
	this->path = remove_adjacent_slashes(ret);
	std::string index_path;
	if (!pathIsFile(this->path) && method == "GET")
	{
		if ((index_path = this->addIndex(request)) != "")
		{
			config = config.get_location_for_request(index_path, location_name);
			this->cgi_pass = config.get_cgi_param();
			this->cgi_param = config.get_cgi_pass();
		}
	}
}