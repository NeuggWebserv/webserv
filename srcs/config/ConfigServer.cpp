#include "ConfigServer.hpp"

// INITIALIZING STATIC MEMBERS

ConfigServer	ConfigServer::default_server = ConfigServer();

parseMap ConfigServer::init_server_map()
{
		    parseMap	myMap;
		    myMap["listen"] = &ConfigServer::add_listen;
		    myMap["root"] = &ConfigServer::add_root;
		    myMap["server_name"] = &ConfigServer::add_server_name;
		    myMap["error_page"] = &ConfigServer::add_error_page;
		    myMap["client_body_buffer_size"] = &ConfigServer::add_client_body_buffer_size;
			myMap["cgi_param"] = &ConfigServer::add_cgi_param;
			myMap["cgi_pass"] = &ConfigServer::add_cgi_pass;
			myMap["allow_methods"] = &ConfigServer::add_allowed_methods;
			myMap["index"] = &ConfigServer::add_index;
			myMap["autoindex"] = &ConfigServer::add_autoindex;
		    return myMap;
}

parseMap ConfigServer::server_parsing_map = ConfigServer::init_server_map();

parseMap ConfigServer::init_location_map()
{
			parseMap     myMap;
			myMap["root"] = &ConfigServer::add_root;
			myMap["error_page"] = &ConfigServer::add_error_page;
			myMap["client_body_buffer_size"] = &ConfigServer::add_client_body_buffer_size;
			myMap["cgi_param"] = &ConfigServer::add_cgi_param;
			myMap["cgi_pass"] = &ConfigServer::add_cgi_pass;
			myMap["allow_methods"] = &ConfigServer::add_allowed_methods;
			myMap["index"] = &ConfigServer::add_index;
			myMap["autoindex"] = &ConfigServer::add_autoindex;
			myMap["alias"] = &ConfigServer::add_alias;
			return myMap;
}

parseMap ConfigServer::location_parsing_map = ConfigServer::init_location_map();

ConfigServer				ConfigServer::init_default_server(const char *filename)
{
	ConfigServer	server;
	fileVector		file;

	file = ConfigReader::readFile(filename);
	if (file.empty()) {
		std::cerr << RED << "Could not open default file at location [" << filename << "]" << RESET << std::endl;
		throw ConfigReader::FileNotFoundException();
	}
	fileVector	begin;
	begin.push_back("server");
	begin.push_back("{");
	file.insert(file.begin(), begin.begin(), begin.end());
	file.insert(file.end(), "}");
	unsigned int	index = 2;
	if (!server.parse_server(index, file)) {
		std::cerr << RED << "Invalid default config file." << RESET << std::endl;
		throw ConfigServer::ExceptionInvalidArguments();
	}
	ConfigServer::default_server = server;
	return server;
}

// CONSTRUCTORS

ConfigServer::ConfigServer(void):
root(""),
client_body_buffer_size(0),
autoindex(false),
alias_set(false)
{
	return ;
}

ConfigServer::ConfigServer(ConfigServer const &src) {
	if (this != &src) {
		this->listen = src.listen;
		this->root = src.root;
		this->server_name = src.server_name;
		this->error_page = src.error_page;
		this->client_body_buffer_size = src.client_body_buffer_size;
		this->cgi_param = src.cgi_param;
		this->cgi_pass = src.cgi_pass;
		this->location = src.location;
		this->allowed_methods = src.allowed_methods;
		this->autoindex = src.autoindex;
		this->index = src.index;
		this->alias = src.alias;
		this->alias_set = src.alias_set;
	}
	return ;
}

ConfigServer::~ConfigServer(void) {
	return ;
}

ConfigServer	&ConfigServer::operator=(ConfigServer const &src) {
	if (this != &src) {
		this->listen = src.listen;
		this->root = src.root;
		this->server_name = src.server_name;
		this->error_page = src.error_page;
		this->client_body_buffer_size = src.client_body_buffer_size;
		this->cgi_param = src.cgi_param;
		this->cgi_pass = src.cgi_pass;
		this->location = src.location;
		this->allowed_methods = src.allowed_methods;
		this->autoindex = src.autoindex;
		this->index = src.index;
		this->alias = src.alias;
		this->alias_set = src.alias_set;
	}
	return *this;
}

int     ConfigServer::parse_server(unsigned int &index, fileVector &file)
{
	fileVector                  args;
	parseMap::iterator          iter;
	std::string                 directive;

	for ( ; index < file.size() && file[index] != "}" ; ++index)
	{
		if ((iter = ConfigServer::server_parsing_map.find(file[index])) != ConfigServer::server_parsing_map.end())
		{
			if (directive != "")
			{
				(this->*ConfigServer::server_parsing_map[directive])(args);
				args.clear();
			}
			directive = iter->first;
		}
		else
		{
			if (directive == "")
				return file[index] == "}" ? 1 : 0;
			else if (file[index] == "location")
			{
				ConfigServer	location;
				std::string		location_name;

				if (directive != "")
				{
					(this->*ConfigServer::server_parsing_map[directive])(args);
					args.clear();
					directive = "";
				}
				++index;
				if (file[index] == "{" || file[index] == "}")		//		locaton_name_check
					throw ConfigServer::ExceptionInvalidForm();
				location_name = file[index];
				++index;
				if (file[index] != "{")
					throw ConfigServer::ExceptionInvalidForm();
				++index;
				if (!location.parse_location(index, file))
					throw ConfigServer::ExceptionInvalidForm();
				this->location[location_name] = location;
				if (file[index] == "}")								//		없어도 똑같지 않나?
					continue ;
			}
			else
				args.push_back(file[index]);
		}
	}
	if (directive != "")
		(this->*ConfigServer::server_parsing_map[directive])(args);
	if (file[index] != "}")
		throw ConfigServer::ExceptionInvalidForm();
	ConfigServer::default_server.pass_members(*this);
	for (std::map<std::string, ConfigServer>::iterator i = this->location.begin() ; i != this->location.end(); i++)
		this->pass_members(i->second);
	return 1;
}

// PASS BLOCK MEMBERS TO CHILD BLOCK
void	ConfigServer::pass_members(ConfigServer &server) const
{
	if (this != &server)
	{
		if (server.listen.empty())
			server.listen.insert(server.listen.begin(), this->listen.begin(), this->listen.end());
		if (server.root == "")
			server.root = this->root;
		server.server_name.insert(server.server_name.end(), this->server_name.begin(), this->server_name.end());
		for (std::map<int, std::string>::const_iterator i = this->error_page.begin(); i != this->error_page.end(); i++) {
			if (server.error_page.find(i->first) == server.error_page.end())
				server.error_page[i->first] = i->second;
		}
		if (server.client_body_buffer_size == 0)
			server.client_body_buffer_size = this->client_body_buffer_size;
		for (std::map<std::string, std::string>::const_iterator i = this->cgi_param.begin() ; i != this->cgi_param.end(); i++) {
			if (server.cgi_param.find(i->first) == server.cgi_param.end())
				server.cgi_param[i->first] = i->second;
		}
		if (server.cgi_pass == "")
			server.cgi_pass = this->cgi_pass;
		if (server.allowed_methods.empty())
			server.allowed_methods = this->allowed_methods;
		server.index.insert(server.index.begin(), this->index.begin(), this->index.end());
	}
	for (std::map<std::string, ConfigServer>::iterator i = server.location.begin(); i != server.location.end(); i++)
		server.pass_members(i->second);
}

int     ConfigServer::parse_location(unsigned int &index, fileVector &file)
{
	fileVector                  args;
	parseMap::iterator          iter;
	std::string                 directive = "";

	for ( ; index < file.size() && file[index] != "}" ; index++)
	{
		if ((iter = ConfigServer::location_parsing_map.find(file[index])) != ConfigServer::location_parsing_map.end())
		{
			if (directive != "")
			{
				(this->*ConfigServer::location_parsing_map[directive])(args);
				args.clear();
			}
			directive = iter->first;
		}
		else
		{
			if (file[index] == "location")
			{
				ConfigServer	location;
				std::string		location_name;

				if (directive != "") {
					(this->*ConfigServer::location_parsing_map[directive])(args);
					args.clear();
					directive = "";
				}
				++index;
				if (file[index] == "{" || file[index] == "}")					//	location name check
					throw ConfigServer::ExceptionInvalidForm();
				location_name = file[index];
				++index;
				if (file[index] != "{")
					throw ConfigServer::ExceptionInvalidForm();
				++index;
				if (!location.parse_location(index, file))
					throw ConfigServer::ExceptionInvalidForm();
				this->location[location_name] = location;
				if (file[index] == "}")											//	필요하냐고??
					continue ;
			}
			else if (directive == "")
				return file[index] == "}" ? 1 : 0;
			else
				args.push_back(file[index]);
		}
	}
	if (directive != "")
		(this->*ConfigServer::location_parsing_map[directive])(args);
	if (file[index] != "}")
		throw ConfigServer::ExceptionInvalidForm();
	return 1;
}

//	ADDMEMBER FUNCTIONS

void        ConfigServer::add_listen(std::vector<std::string> args)
{
	t_listen    listen;
	size_t      separator;

	if (args.size() != 1)
		throw ConfigServer::ExceptionInvalidArguments();
	if ((separator = args[0].find(":")) == std::string::npos)
	{
		if (is_digits(args[0])) {
			listen.host = 0;
			listen.port = std::atoi(args[0].c_str());
			for (std::vector<t_listen>::const_iterator it = listen.begin(); it != listen.end(); it++)
			{
				if (it->port == listen.port)
					throw ConfigServer::ExceptionInvalidArguments();
			}
			this->listen.push_back(listen);
			return ;
		}
		throw ConfigServer::ExceptionInvalidArguments();
	}
	else
	{
		listen.host = str_to_ip(args[0].substr(0, separator));
		separator++;
		std::string	portStr = args[0].substr(separator);
		if (is_digits(portStr))
		{
			listen.port = std::atoi(portStr.c_str());
			this->listen.push_back(listen);
			return ;
		}
		throw ConfigServer::ExceptionInvalidArguments();
	}
}

void        ConfigServer::add_root(std::vector<std::string> args)
{
	if (args.size() != 1 || this->root != "")
		throw ConfigServer::ExceptionInvalidArguments();
	this->root = args[0];
}

void        ConfigServer::add_server_name(std::vector<std::string> args)
{
	if (args.size() == 0)
		throw ConfigServer::ExceptionInvalidArguments();
	for (unsigned int i = 0; i < args.size(); i++)
		this->server_name.push_back(args[i]);
}

void        ConfigServer::add_error_page(std::vector<std::string> args)
{
	std::vector<int>	codes;
	std::string			uri = "";
	size_t				len = args.size();

	for (size_t i = 0; i < len; i++)
	{
		if (is_digits(args[i]))
			codes.push_back(std::atoi(args[i].c_str()));
		else if (codes.empty())
			throw ConfigServer::ExceptionInvalidArguments();
		else if (i == len - 1)
			uri = args[i];
		else
			throw ConfigServer::ExceptionInvalidArguments();
	}
	if (uri == "")
		throw ConfigServer::ExceptionInvalidArguments();
	for (std::vector<int>::iterator i = codes.begin(); i != codes.end(); i++)
		this->error_page[*i] = uri;
}

void        ConfigServer::add_client_body_buffer_size(std::vector<std::string> args)
{
	if (args.size() != 1 || !is_digits(args[0]))
		throw ConfigServer::ExceptionInvalidArguments();
	this->client_body_buffer_size = std::atoi(args[0].c_str());
}

void		ConfigServer::add_cgi_param(std::vector<std::string> args)
{
	if (args.size() != 2)
		throw ConfigServer::ExceptionInvalidArguments();

	this->cgi_param[args[0]] = args[1];
}

void    	ConfigServer::add_cgi_pass(std::vector<std::string> args)
{
	if (args.size() != 1)
		throw ConfigServer::ExceptionInvalidArguments();
	this->cgi_pass = args[0];
}

void		ConfigServer::add_allowed_methods(std::vector<std::string> args)
{
	if (args.empty())
		throw ConfigServer::ExceptionInvalidArguments();
	this->allowed_methods.clear();
	for (fileVector::iterator i = args.begin(); i != args.end(); i++)
	{
		this->allowed_methods.insert(*i);
	}
}

void	ConfigServer::add_index(std::vector<std::string> args)
{
	if (args.empty())
		throw ConfigServer::ExceptionInvalidArguments();
	this->index.insert(this->index.end(), args.begin(), args.end());
}

void	ConfigServer::add_autoindex(std::vector<std::string> args)
{
	if (args.size() != 1)
		throw ConfigServer::ExceptionInvalidArguments();
	if (args[0] == "on")
		this->autoindex = true;
	else if (args[0] == "off")
		this->autoindex = false;
	else
		throw ConfigServer::ExceptionInvalidArguments();
}

void	ConfigServer::add_alias(std::vector<std::string> args)
{
	if (args.size() > 1)
		throw ConfigServer::ExceptionInvalidArguments();
	if (args.size())
		this->alias = args[0];
	this->alias_set = true;
}


// STREAM OPERATOR
std::ostream	&operator<<(std::ostream &out, const ConfigServer &server)
{
	out << "Listen:" << std::endl;
	for (size_t i = 0; i < server.listen.size(); i++)
		out << "\thost: " << server.listen[i].host << " port: " << server.listen[i].port << std::endl;
	out << "root: " << server.root << std::endl;
	out << "server_name: ";
	for (size_t i = 0; i < server.server_name.size(); i++)
	{
		out << server.server_name[i];
		if (i != server.server_name.size() - 1)
			out << " ";
	}
	out << std::endl<< "error_page:" << std::endl;
	for (std::map<int, std::string>::const_iterator i = server.error_page.begin(); i != server.error_page.end(); i++)
		out << "\t" << i->first << " " << i->second << std::endl;
	out << "client_body_buffer_size: " << server.client_body_buffer_size << std::endl;
	out << "cgi_param:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator i = server.cgi_param.begin(); i != server.cgi_param.end(); i++)
		out << "\t" << i->first << " = " << i->second << std::endl;
	out << "cgi_pass:	" << server.cgi_pass << std::endl;
	out << "allowed methods: ";
	for (std::set<std::string>::iterator i = server.allowed_methods.begin(); i != server.allowed_methods.end(); i++)
		out << " " << *i;
	out << std::endl;
	out << "autoindex " << (server.autoindex ? "on" : "off") << std::endl;
	out << "index: ";
	for (fileVector::const_iterator i = server.index.begin(); i != server.index.end(); i++)
		out << *i << " ";
	out << std::endl;
	out << "alias: " << server.alias << std::endl;
	for (std::map<std::string, ConfigServer>::const_iterator i = server.location.begin(); i != server.location.end(); i++)
	{
		out << std::endl << "LOCATION " << i->first << std::endl;
		out << i->second << std::endl;
	}
	return out;
}

// EXCEPTION HANDLING
const char		*ConfigServer::ExceptionInvalidArguments::what()
 const throw() {
	return "Exception: invalid arguments in configuration file";
}

const char		*ConfigServer::ExceptionInvalidForm::what()
 const throw() {
	return "Exception: invalid form in configuration file";
}


// GETERS
std::vector<t_listen>				ConfigServer::get_listen() const
{
	return this->listen;
}
std::string							ConfigServer::get_root() const
{
	return this->root;
}
std::vector<std::string>   			ConfigServer::get_server_name() const
{
	return this->server_name;
}
std::map<int, std::string>			ConfigServer::get_error_page() const
{
	return this->error_page;
}
int									ConfigServer::get_client_body_buffer_size() const
{
	return this->client_body_buffer_size;
}
std::map<std::string, std::string>	ConfigServer::get_cgi_param() const
{
	return this->cgi_param;
}
std::string							ConfigServer::get_cgi_pass() const
{
	return this->cgi_pass;
}
std::map<std::string, ConfigServer>	ConfigServer::get_location() const
{
	return this->location;
}

std::set<std::string>				ConfigServer::get_allowed_methods() const
{
	return this->allowed_methods;
}

std::vector<std::string>			ConfigServer::get_index() const
{
	return this->index;
}

bool								ConfigServer::get_autoindex() const
{
	return this->autoindex;
}

std::string							ConfigServer::get_alias() const
{
	return this->alias;
}

bool								ConfigServer::get_alias_set() const
{
	return this->alias_set;
}

ConfigServer						&ConfigServer::get_default_server()
{
	return ConfigServer::default_server;
}

// GET CONFIG FOR HTTP REQUEST
// ConfigServer						ConfigServer::get_location_for_request(std::string const path, std::string &ret_location_path)
// {
// 	std::string::size_type							try_len = path.length();
// 	std::map<std::string, ConfigServer>::iterator	iter;
// 	std::string										try_location;

// 	if (!try_len)
// 		return *this;
// 	if (!this->location.empty()) {
// 		do {
// 			try_location = path.substr(0, try_len);
// 			iter = this->location.find(try_location);
// 			if (iter != this->location.end() && iter->first[0] != '*') {
// 				ret_location_path = try_location;
// 				return iter->second.get_location_for_request(path, ret_location_path);
// 			}
// 			try_len--;
// 		} while (try_len);
// 		for (std::map<std::string, ConfigServer>::iterator i = this->location.begin(); i != this->location.end(); i++)
// 		{
// 			if (i->first[0] == '*')
// 			{
// 				std::string	suffix = i->first.substr(1);
// 				if (path.length() > suffix.length() && !path.compare(path.length() - suffix.length(), suffix.length(), suffix))
// 				{
// 					return i->second.get_location_for_request(path, ret_location_path);
// 				}
// 			}
// 		}
// 	}
// 	return (*this);
// }
