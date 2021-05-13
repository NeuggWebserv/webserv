#include "Request.hpp"

std::vector<std::string>		Request::init_methods()
{
	std::vector<std::string>	mthds;

	mthds.push_back("GET");
	mthds.push_back("HEAD");
	mthds.push_back("POST");
	mthds.push_back("PUT");
	mthds.push_back("DELETE");
	mthds.push_back("OPTIONS");
	mthds.push_back("TRACE");

	return mthds;
}

std::vector<std::string>	Request::methods = Request::init_methods();

/*
**	CANONICAL
*/

Request::Request(const std::string& str) :
method(""), 
version(""), 
ret(200), 
body(""), 
port(80), 
path(""), 
query(""), 
raw(str)
{
	this->reset_headers();
	this->env_for_cgi.clear();
	this->pull(str);
	if (this->ret != 200)
		std::cerr << "Request Parse Error: " << this->ret << std::endl;
}

Request::~Request()
{
}

Request&	Request::operator=(const Request& obj)
{
	this->headers = obj.get_headers();
	this->env_for_cgi = obj.get_env();
	this->method = obj.get_method();
	this->version = obj.get_version();
	this->ret = obj.get_ret();
	this->body = obj.get_body();
	this->port = obj.get_port();
	this->path = obj.get_path();
	return *this;
}

std::ostream&	operator<<(std::ostream& os, const Request& re)
{
	std::map<std::string, std::string>::const_iterator it;

	os << "Metohd : " << re.get_method() << " |\tHTTP version : " << re.get_version() << "\n" \
	<< "Port : " << re.get_port() << '\n' << "Path : " << re.get_path() << '\n';

	for (it = re.get_headers().begin(); it != re.get_headers().end(); it++)
		os << it->first << ": " << it->second << '\n';
	os << '\n' << "Request body : " << re.get_body() << '\n';

	return (os);
}

/*
**	GETTERS
*/

const std::map<std::string, std::string>&	Request::get_headers() const
{
	return this->headers;
}

const std::map<std::string, std::string>&	Request::get_env() const
{
	return this->env_for_cgi;
}

const std::string&	Request::get_method() const
{
	return this->method;
}

const std::string&	Request::get_version() const
{
	return this->version;
}

int					Request::get_ret() const
{
	return this->ret;
}

const std::string&	Request::get_body() const
{
	return this->body;
}

int					Request::get_port() const
{
	return this->port;
}

const std::string&			Request::get_path() const
{
	return this->path;
}

const std::string&			Request::get_query() const
{
	return this->query;
}

const std::string&			Request::get_raw() const
{
	return this->raw;
}

const std::list<std::pair<std::string, float> >&	Request::get_lang() const
{
	return this->lang;
}


/*
**	SETTERS
*/

void	Request::set_body(const std::string& str)
{
	char	strip[] = {'\n', '\r'};

	this->body.assign(str);
	for (int i = 0; i < 4; i++)
		if (this->body.size() > 0 && this->body[this->body.size() - 1] == strip[i % 2])
			pop_str_tail(this->body);
		else
			break ;
}

void	Request::set_ret(int ret)
{
	this->ret = ret;
}

void	Request::set_method(const std::string &method)
{
	this->method = method;
}