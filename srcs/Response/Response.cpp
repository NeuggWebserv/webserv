#include "Response.hpp"

std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	Response::init_methods()
{
	std::map<std::string, void (Response::*)(Request &, RequestConfig &)> map;

	map["GET"] = &Response::get_method;
	map["HEAD"] = &Response::head_method;
	map["POST"] = &Response::post_method;
	map["PUT"] = &Response::put_method;
	map["DELETE"] = &Response::delete_method;
	map["OPTIONS"] = &Response::options_method;
	map["TRACE"] = &Response::trace_method;

	return map;
}

std::map<std::string, void (Response::*)(Request &, RequestConfig &)> Response::method = Response::init_methods();

// Member functions

void			Response::call(Request & request, RequestConfig & requestConf)
{
	error_map = requestConf.get_error_page();
	is_autoindex = requestConf.get_autoindex();
	code = request.get_ret();
	hostPort.host = requestConf.get_host_port().host;
	hostPort.port = requestConf.get_host_port().port;
	path = requestConf.get_path();

	if (requestConf.get_allowed_methods().find(request.get_method()) == requestConf.get_allowed_methods().end())
		_code = 405;
	else if (requestConf.get_client_body_buffer_size() < request.get_body().size())
		_code = 413;

	if (_code == 405 || _code == 413)
	{
		ResponseHeader	head;

		response = head.not_allowed(requestConf.get_allowed_methods(), requestConf.get_content_location(), _code, requestConf.get_lang()) + "\r\n";
		return ;
	}


	(this->*Response::method_map[request.get_method()])(request, requestConf);
}

// Methods
void			Response::get_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;

	if (requestConf.get_cgi_pass() != "")
	{
		CgiHandler	cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = response.size() - 2;

		response = cgi.executeCgi(requestConf.get_cgi_pass());

		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n", i) == i)
		{
			std::string	str = response.substr(i, response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (response.find("\r\n", j) == j)
			j -= 2;

		response = response.substr(i, j - i);
	}
	else if  (_code == 200)
		_code = readContent();
	else
		response = this->readHtml(_errorMap[_code]);
	if (_code == 500)
		response = this->readHtml(_errorMap[_code]);

	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n" + response;
}

void			Response::head_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;
	(void)request;

	_code = readContent();
	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n";
}

void			Response::post_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;

	if (requestConf.get_cgi_pass() != "")
	{
		CgiHandler	cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = response.size() - 2;

		response = cgi.executeCgi(requestConf.get_cgi_pass());

		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n", i) == i)
		{
			std::string	str = response.substr(i, response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (response.find("\r\n", j) == j)
			j -= 2;

		response = response.substr(i, j - i);
	}
	else
	{
		_code = 204;
		response = "";
	}
	if (_code == 500)
		response = this->readHtml(_errorMap[_code]);
	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n" + response;
}

void			Response::put_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;
	std::string		content;

	content = request.get_body();
	response = "";
	_code = writeContent(content);
	if (_code != 201 && _code != 204)
		response = this->readHtml(_errorMap[_code]);
	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n" + response;
}

void			Response::delete_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;
	(void)request;

	response = "";
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 403 || _code == 404)
		response = this->readHtml(_errorMap[_code]);
	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n" + response;


}

void			Response::options_method(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;
	(void)request;

	_code = readContent();
	response = head.getHeader(response.size(), _path, _code, _type, requestConf.get_content_location(), requestConf.get_lang()) + "\r\n";

}

void			Response::trace_method(Request & request, RequestConfig & requestConf)
{
	(void)requestConf;
	response = request.getRaw();
}

// Utils

int				Response::readContent(void)
{
	std::ifstream		file;
	std::stringstream	buffer;

	response = "";

	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			response = this->readHtml(_errorMap[403]);
			return (403);
		}

		buffer << file.rdbuf();
		response = buffer.str();

		file.close();
	}
	else if (_isAutoIndex) {
		buffer << AutoIndexGenerator::getPage(_path.c_str(),\
			to_string(_hostPort.host), _hostPort.port);
		response = buffer.str();
		_type = "text/html";
	}
	else
	{
		response = this->readHtml(_errorMap[404]);
		return (404);
	}

	return (200);
}

int				Response::writeContent(std::string content)
{
	std::ofstream	file;

	if (pathIsFile(_path))
	{
		file.open(_path.c_str());
		file << content;
		file.close();
		return (204);
	}
	else
	{
		file.open(_path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);

		file << content;
		file.close();
		return (201);
	}
}

std::string		Response::readHtml(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (pathIsFile(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

int				Response::fileExists(std::string path) //deprecated, replaced by ::pathIsFile()
{
	struct stat		stats;

	if (stat(path.c_str(), &stats) == 0)
		return (1);
	return (0);
}

// Getter functions

std::string		Response::getResponse(void)
{
	return (response);
}

// Overloaders

Response & Response::operator=(const Response & src)
{
	response = src.response;
	_path = src._path;
	_code = src._code;
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
