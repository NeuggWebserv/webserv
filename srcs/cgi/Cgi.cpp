/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 19:04:40 by youlee            #+#    #+#             */
/*   Updated: 2021/05/13 19:04:41 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(Request &request, ConfigRequest &config):body(request.get_body())
{
	this->init_env(request, config);
}
Cgi::Cgi(Cgi const &src)
{
	if (this != &src)
	{
		this->body = src.body;
		this->env = src.env;
	}
	return ;
}
Cgi::~Cgi(void)
{
	return ;
}
Cgi		&Cgi::operator=(Cgi const &src)
{
	if (this != &src)
	{
		this->body = src.body;
		this->env = src.env;
	}
	return (*this);
}
void	Cgi::init_env(Request &request, ConfigRequest &config)
{
	std::map<std::string, std::string>		headers = request.get_headers();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->env["AUTH_TYPE"] = headers["Authorization"];
	
	this->env["REDIRECT_STATUS"] = "200";
	this->env["GATAWAY_INTERFACE"] = "CGI/1.1";
	this->env["SCRIPT_NAME"] = config.get_path();
	this->env["SCRIPT_FILENAME"] = config.get_path();
	this->env["REQUEST_METHOD"] = request.get_method();
	this->env["CONTENT_LENGTH"] = to_string(this->body.length());
	this->env["CONTENT_TYPE"] = headers["Content-Type"];
	this->env["PATH_INFO"] = request.get_path();
	this->env["PATH_TRANSLATED"] = request.get_path();
	this->env["QUERY_STRING"] = request.get_query();
	this->env["REMOTEaddr"] = to_string(config.get_host_port().host);
	this->env["REMOTE_IDENT"] = headers["Authorization"];
	this->env["REMOTE_USER"] = headers["Authorization"];
	this->env["REQUEST_URI"] = request.get_path() + request.get_query();
	if (headers.find("Hostname") != headers.end())
		this->env["SERVER_NAME"] = headers["Hostname"];
	else
		this->env["SERVER_NAME"] = this->env["REMOTEaddr"];
	this->env["SERVER_PORT"] = to_string(config.get_host_port().port);
	this->env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->env["SERVER_SOFTWARE"] = "Weebserv/1.0";

	this->env.insert(config.get_cgi_param().begin(), config.get_cgi_param().end());
}

char	**Cgi::get_env_as_cstr_array() const
{
	char	**env = new char*[this->env.size() + 1];
	int		j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->env.begin();i != this->env.end();i++)
	{
		std::string		element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return (env);
}

std::string	Cgi::execute_cgi(const std::string &script_name)
{
	pid_t			pid;
	int				save_stdin;
	int				save_stdout;
	char			**env;
	std::string		new_body;

	try
	{
		env = this->get_env_as_cstr_array();
	}
	catch(const std::bad_alloc& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);

	FILE			*file_in = tmpfile();
	FILE			*file_out = tmpfile();
	long			fd_in = fileno(file_in);
	long			fd_out = fileno(file_out);
	int				ret = 1;

	write(fd_in, body.c_str(), body.size());
	lseek(fd_in, 0, SEEK_SET);

	pid = fork();

	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const *nll = NULL;

		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		execve(script_name.c_str(), nll, env);
		std::cerr<< "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status; 500\r\n\r\n", 15);
	}
	else
	{
		char	buf[CGI_BUF_SIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fd_out, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buf, 0, CGI_BUF_SIZE);
			ret = read(fd_out, buf, CGI_BUF_SIZE - 1);
			new_body += buf;
		}
	}

	dup2(save_stdin,STDIN_FILENO);
	dup2(save_stdout,STDOUT_FILENO);
	fclose(file_in);
	fclose(file_out);
	close(fd_in);
	close(fd_out);
	close(save_stdout);
	close(save_stdin);

	for (size_t i = 0; env[i];i++)
		delete[] env[i];
	delete[] env;

	if (!pid) exit(0);
	return (new_body);
}