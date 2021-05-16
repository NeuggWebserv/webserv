#include "Response.hpp"

std::map<std::string, void (Response::*)(Request &, ConfigRequest &)>	Response::init_methods()
{
	std::map<std::string, void (Response::*)(Request &, ConfigRequest &)> map;

	map["GET"] = &Response::get_method;
	map["HEAD"] = &Response::head_method;
	map["POST"] = &Response::post_method;
	map["PUT"] = &Response::put_method;
	map["DELETE"] = &Response::delete_method;
	map["OPTIONS"] = &Response::options_method;
	map["TRACE"] = &Response::trace_method;

	return map;
}

std::map<std::string, void (Response::*)(Request &, ConfigRequest &)> Response::method_map = Response::init_methods();

// Member functions

void			Response::call(Request & request, ConfigRequest & requestConf)
{
	error_map = requestConf.get_error_page();
	is_autoindex = requestConf.get_autoindex();
	code = request.get_ret();
	host_port.host = requestConf.get_host_port().host;
	host_port.port = requestConf.get_host_port().port;
	path = requestConf.get_path();

	if (requestConf.get_allowed_method().find(request.get_method()) == requestConf.get_allowed_method().end())
		code = 405;
	else if (requestConf.get_cli_body_size() < request.get_body().size())
		code = 413;

	if (code == 405 || code == 413)
	{
		ResponseHeader	head;

		response = head.not_allowed(requestConf.get_allowed_method(), requestConf.get_content_location(), code, requestConf.get_len()) + "\r\n";
		return ;
	}


	(this->*Response::method_map[request.get_method()])(request, requestConf);
}

// Methods
void			Response::get_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;

	if (requestConf.get_cgi_pass() != "")
	{
		Cgi			cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = response.size() - 2;

		response = cgi.execute_cgi(requestConf.get_cgi_pass());

		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n", i) == i)
		{
			std::string	str = response.substr(i, response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (response.find("\r\n", j) == j)
			j -= 2;

		response = response.substr(i, j - i);
	}
	else if  (code == 200)
		code = read_content();
	else
		response = this->read_html(error_map[code]);
	if (code == 500)
		response = this->read_html(error_map[code]);

	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n" + response;
}

void			Response::head_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;
	(void)request;

	code = read_content();
	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n";
}

void			Response::post_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;

	if (requestConf.get_cgi_pass() != "")
	{
		Cgi			cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = response.size() - 2;

		response = cgi.execute_cgi(requestConf.get_cgi_pass());

		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n", i) == i)
		{
			std::string	str = response.substr(i, response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (response.find("\r\n", j) == j)
			j -= 2;

		response = response.substr(i, j - i);
	}
	else
	{
		code = 204;
		response = "";
	}
	if (code == 500)
		response = this->read_html(error_map[code]);
	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n" + response;
}

void			Response::put_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;
	std::string		content;

	content = request.get_body();
	response = "";
	code = write_content(content);
	if (code != 201 && code != 204)
		response = this->read_html(error_map[code]);
	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n" + response;
}

void			Response::delete_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;
	(void)request;

	response = "";
	if (path_is_file(path))
	{
		if (remove(path.c_str()) == 0)
			code = 204;
		else
			code = 403;
	}
	else
		code = 404;
	if (code == 403 || code == 404)
		response = this->read_html(error_map[code]);
	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n" + response;


}

void			Response::options_method(Request & request, ConfigRequest & requestConf)
{
	ResponseHeader	head;
	(void)request;

	code = read_content();
	response = head.get_header(response.size(), path, code, type, requestConf.get_content_location(), requestConf.get_len()) + "\r\n";

}

void			Response::trace_method(Request & request, ConfigRequest & requestConf)
{
	(void)requestConf;
	response = request.get_raw();
}

// Utils

int				Response::read_content(void)
{
	std::ifstream		file;
	std::stringstream	buffer;

	response = "";

	if (path_is_file(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			response = this->read_html(error_map[403]);
			return (403);
		}

		buffer << file.rdbuf();
		response = buffer.str();

		file.close();
	}
	else if (is_autoindex) {
		buffer << AutoIndex::get_page(path.c_str(),\
			to_string(host_port.host), host_port.port);
		response = buffer.str();
		type = "text/html";
	}
	else
	{
		response = this->read_html(error_map[404]);
		return (404);
	}

	return (200);
}

int				Response::write_content(std::string content)
{
	std::ofstream	file;

	if (path_is_file(path))
	{
		file.open(path.c_str());
		file << content;
		file.close();
		return (204);
	}
	else
	{
		file.open(path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);

		file << content;
		file.close();
		return (201);
	}
}

std::string		Response::read_html(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (path_is_file(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

int				Response::file_exists(std::string path) //deprecated, replaced by ::path_is_file()
{
	struct stat		stats;

	if (stat(path.c_str(), &stats) == 0)
		return (1);
	return (0);
}

// Getter functions

std::string		Response::get_response(void)
{
	return (response);
}

// Overloaders

Response & Response::operator=(const Response & src)
{
	response = src.response;
	path = src.path;
	code = src.code;
	return (*this);
}

// Constructors and destructors

Response::Response(void)
{
}

Response::Response(const Response & src)
{
	*this = src;
}

Response::~Response(void)
{
}
