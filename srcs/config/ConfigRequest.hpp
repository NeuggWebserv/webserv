/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:50:33 by youlee            #+#    #+#             */
/*   Updated: 2021/04/19 19:50:35 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGREQUEST_HPP
#define CONFIGREQUEST_HPP

#include "webserv.hpp"
#include "Config.hpp"
#include "Request.hpp"

class ConfigRequest
{
	public:
		ConfigRequest(void);
		ConfigRequest(ConfigRequest const &cr);
		ConfigRequest(ConfigServer &config, Request &request, const std::string &path,
						const std::string &method, std::string &location_name);
		virtual ~ConfigRequest(void);

		ConfigRequest								&operator=(ConfigRequest const &cr);
		friend std::ostream							&operator<<(std::ostream &os, ConfigRequest &cr);

		const std::string							&get_content_location() const;
		const std::string							&get_path() const;
		const std::map<int, std::string>			&get_error_page() const;
		const unsigned long							&get_cli_body_size() const;
		const std::map<std::string, std::string>	&get_cgi_param() const;
		const std::string							&get_cgi_pass() const;
		const std::set<std::string>					&get_allowed_method() const;
		const t_listen								&get_host_port() const;
		const std::string							&get_len() const;
		const std::vector<std::string>				&get_index() const;
		const bool									&get_autoindex() const;

		void										set_path(int code);
		void										set_path(const std::string&);
		void										set_content_location(const std::string&);
		void										set_host_port(const t_listen hostport);

		std::string									add_index(Request &request);
	private:
		std::string									content_location;
		std::string									path;
		std::map<int, std::string>					error_page;
		unsigned long								cli_body_size;
		std::map<std::string, std::string>			cgi_param;
		std::string									cgi_pass;
		std::set<std::string>						allowed_method;
		t_listen									host_port;
		std::string									len;
		std::vector<std::string>					index;
		bool										auto_index;
};

#endif