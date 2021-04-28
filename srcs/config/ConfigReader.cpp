/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 16:17:29 by youlee            #+#    #+#             */
/*   Updated: 2021/04/28 16:17:30 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigReader.hpp"

ConfigReader::ConfigReader(void){ }

ConfigReader::ConfigReader(ConfigReader const &Co)
{
	if (this == &Co)
		return ;
	*this = Co;
}

ConfigReader::~ConfigReader(void) {}

ConfigReader	&ConfigReader::operator=(ConfigReader const &Co)
{
	if (this == &Co)
		return (*this);
	*this = Co;
	return (*this);
}

fileVector		ConfigReader::split(std::string str, std::string charset)
{
	fileVector	val;

	str+=charset[0]; // ' ' insert
	std::string::size_type start_idx = str.find_first_not_of(charset, 0);
	std::string::size_type end_idx = 0;
	while (1)
	{
		end_idx = str.find_first_of(charset, start_idx);
		if (end_idx == std::string::npos)
			break ; // end index not found break point
		std::string	sub = str.substr(start_idx, end_idx - start_idx);
		val.push_back(sub);
		if ((start_idx = str.find_first_not_of(charset, end_idx)) == std::string::npos)
			break ; // end index not found charset
	}
	return (val); //return fileVector
}

fileVector		ConfigReader::readFile(const char *file_name)
{
	int			res = BUFFER_SIZE;
	char		buf[BUFFER_SIZE + 1];
	std::string	contents="";
	int			fd;
	fileVector	file;

	//init buf array
	for (int i=0;i<BUFFER_SIZE;i++)
		buf[i] = '\0';
	if ((fd = open(file_name,O_RDONLY)) <= 0)
		throw (ConfigReader::FileNotFoundException());
	res = BUFFER_SIZE;
	while(res)
	{
		if (!(res = read(fd, buf, BUFFER_SIZE)))
			throw (ConfigReader::ReadFunctionException());
		else
		{
			buf[res] = '\0';
			contents += buf;
		}
	}
	file = ConfigReader::split(contents, std::string(" \n\t"));
	return (file);
}

const char *ConfigReader::FileNotFoundException::what() const throw()
{
	return ("File Not Found Exception!\n");
}

const char *ConfigReader::ReadFunctionException::what() const throw()
{
	return ("Read file Error Exception!\n");
}