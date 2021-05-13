/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 19:04:34 by youlee            #+#    #+#             */
/*   Updated: 2021/05/13 19:04:36 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include "ConfigRequest.hpp"

# define CGI_BUF_SIZE 65536

class Cgi
{
	private:
		Cgi(void);
		void								init_env(Request &request, ConfigRequest &config);
		char								**get_env_as_cstr_array() const;
		int									get_socket(unsigned int port);
		int									connect_socket(unsigned int port);
		std::map<std::string, std::string>	env;
		std::string							body;
	public:
		//set up env according to the request
		Cgi(Request &request, ConfigRequest &config);
		Cgi(Cgi const &src);
		virtual ~Cgi(void);

		Cgi									&operator=(Cgi const &src);
		//exectue cgi and return body
		std::string							execute_cgi(const std::string &script_name);
};
#endif