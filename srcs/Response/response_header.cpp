#include "response_header.hpp"

std::map<int, std::string>	ResponseHeader::errors = init_error_map();

// Member functions

std::string		ResponseHeader::get_header(size_t size, const std::string& path, int code, std::string type, const std::string& content_loc, const std::string& lang)
{
	std::string	header;

	reset_values();
	set_values(size, path, code, type, content_loc, lang);

	header = "HTTP/1.1 " + to_string(code) + " " + get_status_message(code) + "\r\n";
	header += write_header();

	return (header);
}

std::string		ResponseHeader::not_allowed(std::set<std::string> methods, const std::string& path, int code, const std::string& lang)
{
	std::string	header;

	reset_values();
	set_values(0, path, code, "", path, lang);
	set_allow(methods);

	if (code == 405)
		header = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (code == 413)
		header = "HTTP/1.1 413 Payload Too Large\r\n";
	header += write_header();

	return (header);
}

std::string		ResponseHeader::write_header(void)
{
	std::string	header = "";

	if (allow != "")
		header += "Allow: " + allow + "\r\n";
	if (content_language != "")
		header += "Content-Language: " + content_language + "\r\n";
	if (content_length != "")
		header += "Content-Length: " + content_length + "\r\n";
	if (content_location != "")
		header += "Content-Location: " + content_location + "\r\n";
	if (content_type != "")
		header += "Content-Type: " + content_type + "\r\n";
	if (date != "")
		header += "Date: " + date + "\r\n";
	if (last_modified != "")
		header += "Last-Modified: " + last_modified + "\r\n";
	if (location != "")
		header += "Location: " + location + "\r\n";
	if (retry_after != "")
		header += "Retry-After: " + retry_after + "\r\n";
	if (server != "")
		header += "Server: " + server + "\r\n";
	if (transfer_encoding != "")
		header += "Transfer-Encoding: " + transfer_encoding + "\r\n";
	if (www_authenticate != "")
		header += "WWW-Authenticate: " + www_authenticate + "\r\n";

	return (header);
}

std::string		ResponseHeader::get_status_message(int code)
{
	if (errors.find(code) != errors.end())
		return errors[code];
	return ("Unknown Code");
}

std::map<int, std::string>	ResponseHeader::init_error_map()
{
	std::map<int, std::string>	errs;

	errs[100] = "Continue";
	errs[200] = "OK";
	errs[201] = "Created";
	errs[204] = "No Content";
	errs[400] = "Bad Request";
	errs[403] = "Forbidden";
	errs[404] = "Not Found";
	errs[405] = "Method Not Allowed";
	errs[413] = "Payload Too Large";
	errs[500] = "Internal Server Error";

	return errs;
}

void			ResponseHeader::set_values(size_t size, const std::string& path, int code, std::string type, const std::string& content_loc, const std::string& lang)
{
	set_allow();
	set_content_language(lang);
	set_content_length(size);
	set_content_location(content_loc);
	set_content_type(type, path);
	set_date();
	set_last_modified(path);
	set_location(code, path);
	set_retry_after(code, 3);
	set_server();
	set_transfer_encoding();
	set_www_authenticate(code);
}

void			ResponseHeader::reset_values(void)
{
	allow = "";
	content_language = "";
	content_length = "";
	content_location = "";
	content_type = "";
	date = "";
	last_modified = "";
	location = "";
	retry_after = "";
	server = "";
	transfer_encoding = "";
	www_authenticate = "";
	// init_error_map();
}

// Setter functions

void			ResponseHeader::set_allow(std::set<std::string> methods)
{
	std::set<std::string>::iterator it = methods.begin();

	while (it != methods.end())
	{
		allow += *(it++);
		if (it != methods.end())
			allow += ", ";
	}
}

void			ResponseHeader::set_allow(const std::string& allow)
{
	this->allow = allow;
}

void			ResponseHeader::set_content_language(const std::string& lang)
{
	content_language = lang;
}

void			ResponseHeader::set_content_length(size_t size)
{
	content_length = to_string(size);
}

void			ResponseHeader::set_content_location(const std::string& path)
{
	content_location = path;
}

void			ResponseHeader::set_content_type(std::string type, std::string path)
{
	if (type != "")
	{
		content_type = type;
		return ;
	}
	type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	if (type == "html")
		content_type = "text/html";
	else if (type == "css")
		content_type = "text/css";
	else if (type == "js")
		content_type = "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		content_type = "image/jpeg";
	else if (type == "png")
		content_type = "image/png";
	else if (type == "bmp")
		content_type = "image/bmp";
	else
		content_type = "text/plain";
}

void			ResponseHeader::set_date(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	date = std::string(buffer);
}

void			ResponseHeader::set_last_modified(const std::string& path)
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		last_modified = std::string(buffer);
	}
}

void			ResponseHeader::set_location(int code, const std::string& redirect)
{
	if (code == 201 || code / 100 == 3)
	{
		location = redirect;
	}
}

void			ResponseHeader::set_retry_after(int code, int sec)
{
	if (code == 503 || code == 429 || code == 301)
	{
		retry_after = to_string(sec);
	}
}

void			ResponseHeader::set_server(void)
{
	server = "Weebserv/1.0.0 (Unix)";
}

void			ResponseHeader::set_transfer_encoding(void)
{
	transfer_encoding = "identity";
}

void			ResponseHeader::set_www_authenticate(int code)
{
	if (code == 401)
	{
		www_authenticate = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	}
}


// Constructors and destructors

ResponseHeader::ResponseHeader(void)
{
	reset_values();
}

ResponseHeader::~ResponseHeader(void)
{

}

//		Don't use

ResponseHeader & ResponseHeader::operator=(const ResponseHeader & src)
{
	//		forbidden
	(void)src;
	return *this;
}
ResponseHeader::ResponseHeader(const ResponseHeader & src)
{
	//		forbidden
	(void)src;
}
