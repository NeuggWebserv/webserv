#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.hpp"

class Request
{
private:
	std::string									method;
	std::string									version;
	std::map<std::string, std::string>			headers;
	std::map<std::string, std::string>			env_for_cgi;
	int											ret;
	std::string									body;
	int											port;
	std::string									path;
	std::string									query;
	std::list<std::pair<std::string, float> >	lang;
	const std::string&							raw;

	//				PARSING
	int			read_first_line(const std::string& content);
	int			read_path(const std::string& content, size_t i);
	int			read_version(const std::string& content, size_t i);
	int			check_method();
	int			check_port();
	void		find_query();
	std::string format_header_for_cgi(std::string& key);
	std::string	next_line(const std::string &str, size_t& i);
	void		set_lang();


	//				AVAILABLE HTTP METHODS
	static	std::vector<std::string>	methods;

	//				UNAVAILABLE CTORS
	Request();
	Request(const Request&);

public:
	Request(const std::string& str);
	~Request();
	Request&	operator=(const Request&);

	//				GETTERS
	const std::map<std::string, std::string>&			get_headers() const;
	const std::map<std::string, std::string>&			get_env() const;
	const std::string&									get_method() const;
	const std::string&									get_version() const;
	int													get_ret() const;
	const std::string&									get_body() const;
	int													get_port() const;
	const std::string&									get_path() const;
	const std::string&									get_query() const;
	const std::string&									get_raw() const;
	const std::list<std::pair<std::string, float> >&	get_lang() const;

	//				SETTERS
	void	set_body(const std::string& line);
	void	set_ret(int);
	void	set_method(const std::string &method);

	//				UTILS
	int		pull(const std::string& str);
	void	reset_headers();
	void	strip_all();

	static std::vector<std::string>		init_methods();

	class	ExceptionParseError : public std::exception
	{
		virtual const char	*what() const throw();
	};

};

std::ostream&	operator<<(std::ostream& os, const Request& re);

#endif