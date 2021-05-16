
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"

#include "response_header.hpp"
#include "Request.hpp"
#include "ConfigRequest.hpp"
#include "Cgi.hpp"
#include "AutoIndex.hpp"

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
	void			call(Request & request, ConfigRequest & requestConf);

	void			get_method(Request & request, ConfigRequest & requestConf);
	void			head_method(Request & request, ConfigRequest & requestConf);
	void			post_method(Request & request, ConfigRequest & requestConf);
	void			put_method(Request & request, ConfigRequest & requestConf);
	void			delete_method(Request & request, ConfigRequest & requestConf);
	void			options_method(Request & request, ConfigRequest & requestConf);
	void			trace_method(Request & request, ConfigRequest & requestConf);

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

	static std::map<std::string, void (Response::*)(Request &, ConfigRequest &)>	method_map;
	static std::map<std::string, void (Response::*)(Request &, ConfigRequest &)>	init_methods();
};

#endif
