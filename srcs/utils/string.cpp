/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:52:45 by youlee            #+#    #+#             */
/*   Updated: 2021/05/06 16:52:51 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string						strip(std::string &str, char c)
{
	size_t	i;

	if (!str.size())
		return (str);
	i = str.size();
	while (i && str[i - 1] == c)
		i--;
	str.resize(i);
	for (i = 0; str[i] == c; i++)
		str = str.substr(i, std::string::npos);
	return (str);
}

std::string						to_string(size_t n)
{
	std::stringstream temp;

	temp << n;

	return temp.str();
}

std::string&					pop_str_tail(std::string& str)
{
	if (str.size())
		str.resize(str.size() - 1);
	return str;
}

std::string						to_upper(std::string &str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return (str);
}

std::string						to_lower(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return (str);
}

std::string						&capitalize(std::string &str)
{
	size_t			i = 0;

	to_lower(str);
	str[i] = std::toupper(str[i]);
	while((i = str.find_first_of('-', i + 1)) != std::string::npos)
	{
		if (i + 1 < str.size())
			str[i + 1] = std::toupper(str[i + 1]);
	}
	return (str);
}

std::string						read_key(char *line)
{
	std::string		ret;

	for (int i=0; line[i] && line[i] != ':'; i++)
		ret.push_back(line[i]);
	capitalize(ret);
	return (strip(ret, ' '));
}

std::string						read_key(const std::string &line)
{
	std::string		ret;

	size_t i = line.find_first_of(':');
	ret.append(line, 0, i);
	capitalize(ret);
	return (strip(ret, ' '));
}

std::string						read_value(char *line)
{
	int i;
	std::string ret;

	for (i = 0;line[i] && line[i] != ':';i++);
	while (line[++i] && line[i] == ' ');
	i--;
	while (line[++i])
		ret.push_back(line[i]);
	return (strip(ret, ' '));
}

std::string						read_value(const std::string &line)
{
	size_t i;
	std::string ret;

	i = line.find_first_of(':');
	i = line.find_first_not_of(' ', i + 1);
	if (i != std::string::npos)
		ret.append(line, i, std::string::npos);
	return (strip(ret, ' '));
}
