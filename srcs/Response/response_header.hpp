#ifndef RESPONSE_HEADER_HPP
#define RESPONSE_HEADER_HPP

#include "webserv.hpp"

class ResponseHeader
{
public:
	ResponseHeader(void);
	~ResponseHeader(void);

	// Setter functions
	void			set_allow(std::set<std::string> methods);
	void			set_allow(const std::string& allow = "");
	void			set_content_language(const std::string& lang = "");
	void			set_content_length(size_t size);
	void			set_content_location(const std::string& path);
	void			set_content_type(std::string type, std::string path);
	void			set_date(void);
	void			set_last_modified(const std::string& path);
	void			set_location(int code, const std::string& redirect);
	void			set_retry_after(int code, int sec);
	void			set_server(void);
	void			set_transfer_encoding(void);
	void			set_www_authenticate(int code);

	// Member functions
	std::string		get_header(size_t size, const std::string& path, int code, std::string type, const std::string& content_location, const std::string& lang);
	std::string		not_allowed(std::set<std::string> methods, const std::string& path, int code, const std::string& lang);
	std::string		write_header(void);
	void			set_values(size_t size, const std::string& path, int code, std::string type, const std::string& content_location, const std::string& lang);
	void			reset_values(void);
	std::string		get_status_message(int code);

private:
	std::string					allow;
	std::string					content_language;
	std::string					content_length;
	std::string					content_location;
	std::string					content_type;
	std::string					date;
	std::string					last_modified;
	std::string					location;
	std::string					retry_after;
	std::string					server;
	std::string					transfer_encoding;
	std::string					www_authenticate;
	static std::map<int, std::string>	errors;
	std::map<int, std::string>			init_error_map();

	//		Don't use
	ResponseHeader(const ResponseHeader & src);
	ResponseHeader & operator=(const ResponseHeader & src);
};

#endif
