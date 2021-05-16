/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_func.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 19:19:04 by youlee            #+#    #+#             */
/*   Updated: 2021/05/03 19:19:24 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void		Request::reset_headers()
{
	this->headers.clear();

	//init default header value.
	this->headers["Accept-Charsets"] = "";
	this->headers["Accept-Language"] = "";
	this->headers["Allow"] = "";
	this->headers["Auth-Scheme"] = "";
	this->headers["Authorization"] = "";
	this->headers["Content-Language"] = "";
	this->headers["Content-Length"] = "";
	this->headers["Content-Location"] = "";
	this->headers["Content-Type"] = "";
	this->headers["Date"] = "";
	this->headers["Host"] = "";
	this->headers["Last-Modified"] = "";
	this->headers["Location"] = "";
	this->headers["Referer"] = "";
	this->headers["Retry-After"] = "";
	this->headers["Server"] = "";
	this->headers["Transfer-Encoding"] = "";
	this->headers["User-Agent"] = "";
	this->headers["Www-Authenticate"] = "";
	this->headers["Connection"] = "Keep-Alive";
}

int			Request::read_first_line(const std::string &str)
{
	size_t i;
	std::string content;

	i = str.find_first_of('\n');
	content = str.substr(0, i);
	i = content.find_first_of(' ');

	if (i == std::string::npos)
	{
		this->ret = 400;
		std::cerr << "RFL no space after method" << std::endl;
		return (400);
	}
	this->method.assign(content, 0, i);
	return (this->read_path(content, i));
}

int			Request::read_path(const std::string &content, size_t i)
{
	size_t j;

	if ((j = content.find_first_not_of(' ', i)) == std::string::npos)
	{
		this->ret = 400;
		std::cerr << "No Path / HTTP version" << std::endl;
		return (400);
	}
	if ((i = content.find_first_of(' ', j)) == std::string::npos)
	{
		this->ret = 400;
		std::cerr << "No HTTP version" << std::endl;
		return (400);
	}
	this->path.assign(content, j, i - j);
	return (this->read_version(content, i));
}

int			Request::read_version(const std::string& content, size_t i)
{
	std::string sub;

	if ((i = content.find_first_not_of(' ', i)) == std::string::npos)
	{
		this->ret = 400;
		std::cerr << "No HTTP version" << std::endl;
		return (400);
	}
	sub = content.substr(i, i + 4);
	if (sub.substr(0, 5) == "HTTP/")
		this->version.assign(content, i + 5, 3);
	if (this->version != "1.0" && this->version != "1.1")
	{
		this->ret = 400;
		std::cerr << "BAD HTTP VERSION (" << this->version << ")" << std::endl;
		return (this->ret);
	}
	return (this->check_method());
}

int			Request::check_method()
{
	for (size_t i = 0; i < this->methods.size();i++)
		if (this->methods[i] == this->method)
			return (this->ret);
	std::cerr << "Invalid method requested" << std::endl;
	this->ret = 400;
	return (this->ret);
}

int			Request::check_port()
{
	size_t i = this->headers["Host"].find_first_of(':');

	if (i == std::string::npos)
		this->port = 80;
	else
	{
		std::string tmp(this->headers["Host"], i + 1);
		this->port = std::atoi(tmp.c_str());
	}
	return (this->port);
}

std::string	Request::next_line(const std::string &str, size_t &i)
{
	std::string		ret;
	size_t			j;

	if (i == std::string::npos)
		return "";
	j = str.find_first_of('\n', i);
	ret = str.substr(i, j - i);
	if (ret[ret.size() - 1] == '\r')
		pop_str_tail(ret);
	i = (j == std::string::npos ? j : j + 1);
	return (ret);
}

int			Request::pull(const std::string &str)
{
	std::string		key;
	std::string		value;
	std::string		line;
	size_t			i(0);

	this->read_first_line(next_line(str, i));
	while ((line = next_line(str, i)) != "\r" && line != "" && this->ret != 400)
	{
		key = read_key(line);
		value = read_value(line);
		if (this->headers.count(key))
			this->headers[key] = value;
		if (key.find("Secret") != std::string::npos)
			this->env_for_cgi[format_header_for_cgi(key)] = value;
	}
	if (this->headers["Www-Authenticate"] != "")
		this->env_for_cgi["Www-Authenticate"] = this->headers["Www-Authenticate"];
	this->set_lang();
	this->set_body(str.substr(i, std::string::npos));
	this->find_query();
	return (this->ret);
}

void			Request::set_lang()
{
	std::vector<std::string>		token;
	std::string						header;
	size_t							i;

	if ((header = this->headers["Accept-Language"]) != "")
	{
		token = split(header, ',');
		for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); it++)
		{
			float			weight = 0.0;
			std::string		lang;

			lang = (*it).substr(0, (*it).find_first_of('-'));
			strip(lang, ' ');
			if ((i = lang.find_last_of(';')) != std::string::npos)
				weight = atof((*it).substr(i + 4).c_str());
			lang.resize(i > 2 ? 2 : i);
			this->lang.push_back(std::pair<std::string, float>(lang, weight));
		}
		this->lang.sort(compare_langs);
	}
}

void			Request::strip_all()
{
	strip(this->method, '\n');
	strip(this->method, '\r');
	strip(this->method, ' ');
	strip(this->version, '\n');
	strip(this->version, '\r');
	strip(this->version, ' ');
	strip(this->path, '\n');
	strip(this->path, '\r');
	strip(this->path, ' ');
}

void			Request::find_query()
{
	size_t		i;

	i = this->path.find_first_of('?');
	if (i != std::string::npos)
	{
		this->query.assign(this->path, i + 1,std::string::npos);
		this->path = this->path.substr(0, i);
	}
}

std::string		Request::format_header_for_cgi(std::string &key)
{
	str_to_upper(key);
	for (size_t i = 0; i < key.size(); i++)
	{
		if (key[i] == '-')
			key[i] = '_';
	}
	return ("HTTP_" + key);
}