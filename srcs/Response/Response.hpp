
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 14:18:58 by cclaude           #+#    #+#             */
/*   Updated: 2021/03/25 14:08:31 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"

#include "response_header.hpp"
#include "Request.hpp"
#include "ConfigRequest.hpp"
#include "CgiHandler.hpp"
#include "AutoIndexGenerator.hpp"

class Response
{
public:
	Response(void);
	Response(const Response & src);
	~Response(void);

	Response & operator=(const Response & src);

	// Getter functions
	std::string		get_response(void);

	// Member functions
	void			call(Request & request, RequestConfig & requestConf);

	void			get_method(Request & request, RequestConfig & requestConf);
	void			head_method(Request & request, RequestConfig & requestConf);
	void			post_method(Request & request, RequestConfig & requestConf);
	void			put_method(Request & request, RequestConfig & requestConf);
	void			delete_method(Request & request, RequestConfig & requestConf);
	void			options_method(Request & request, RequestConfig & requestConf);
	void			trace_method(Request & request, RequestConfig & requestConf);

	int				read_content(void);
	int				write_content(std::string content);
	int				file_exists(std::string path);
	std::string		read_html(const std::string& path);

private:
	std::string					response;
	std::string					path;
	int							code;
	std::string					type;
	bool						is_autoindex;
	t_listen					host_port;
	std::map<int, std::string>	error_map;

	static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	method_map;
	static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	init_methods();
};

#endif
