/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:50:42 by youlee            #+#    #+#             */
/*   Updated: 2021/04/19 19:50:43 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include "webserv.hpp"
#include "Config.hpp"
// #include "TestConfigServer/test.hpp"

# define parseMap std::map<std::string, void (ConfigServer::*)(fileVector)>

class ConfigServer
{
	public:
		// *	DEFAULT
		ConfigServer(void);
		ConfigServer(ConfigServer const &src);
		virtual ~ConfigServer(void);
		ConfigServer						&operator=(ConfigServer const &src);

		// *	GETERS
		//	ONLY SERVER
		std::vector<t_listen>				get_listen() const;
		std::vector<std::string>   			get_server_name() const;
		//	BOTH SERVER, LOCATION
		std::string							get_root() const;
		std::map<int, std::string>			get_error_page() const;
		int									get_client_body_buffer_size() const;
		std::map<std::string, std::string>	get_cgi_param() const;
		std::string							get_cgi_pass() const;
		std::map<std::string, ConfigServer> get_location() const;
		std::set<std::string>				get_allowed_methods() const;
		std::vector<std::string>			get_index() const;
		bool								get_autoindex() const;
		//	ONLY LOCATION
		std::string							get_alias() const;
		bool								get_alias_set() const;


		// *	EXCEPTION
		class	ExceptionInvalidArguments: public std::exception {
			virtual const char	*what() const throw();
		};
		class	ExceptionInvalidForm: public std::exception {
			virtual const char	*what() const throw();
		};

		// *	OTHERS
		int									parse_server(unsigned int &i, std::vector<std::string> &file);
		void								pass_members(ConfigServer &server) const;
		static ConfigServer					init_default_server(const char *filename);
		static ConfigServer					&get_default_server();
		// RETURN CONFIG ACCORDING TO URI
		// ConfigServer						get_location_for_request(std::string const path, std::string &locationPath);

		// *	FOR TEST
		friend std::ostream					&operator<<(std::ostream &out, const ConfigServer &server);

	private:
		// *	DIRECTIVES
		//	ONLY SERVER
		std::vector<t_listen>				listen;
		std::vector<std::string>			server_name;
		//	BOTH SERVER, LOCATION
		std::string							root;
		std::map<int, std::string>			error_page; // error page redirections
		int									client_body_buffer_size; // max size for the client body, defaults to 8 000
		std::map<std::string, std::string>	cgi_param;
		std::string							cgi_pass;
		std::map<std::string, ConfigServer>	location;
		std::set<std::string>				allowed_methods;
		std::vector<std::string>			index;
		bool								autoindex;
		//	ONLY LOCATION
		std::string							alias;
		bool								alias_set;

		// *	ADD MEMBER FUNCTIONS
		void    							add_listen(std::vector<std::string> args);
		void    							add_server_name(std::vector<std::string> args);
		void    							add_root(std::vector<std::string> args);
		void    							add_error_page(std::vector<std::string> args);
		void    							add_client_body_buffer_size(std::vector<std::string> args);
		void								add_cgi_param(std::vector<std::string> args);
		void    							add_cgi_pass(std::vector<std::string> args);
		void								add_allowed_methods(std::vector<std::string> args);
		void								add_index(std::vector<std::string> args);
		void								add_autoindex(std::vector<std::string> args);
		void								add_alias(std::vector<std::string> args);

		// *	SET PARSEMAP
		static parseMap						server_parsing_map;
		static parseMap						location_parsing_map;
		static parseMap 					init_server_map();
		static parseMap 					init_location_map();

		// *	OTHERS
		static ConfigServer					default_server;
		int									parse_location(unsigned int &i, std::vector<std::string> &file);

};

#endif
