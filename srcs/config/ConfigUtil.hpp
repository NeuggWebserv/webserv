/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtil.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:50:59 by youlee            #+#    #+#             */
/*   Updated: 2021/04/19 19:51:00 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGUTIL_HPP
#define CONFIGUTIL_HPP

# include "webserv.hpp"

# define parseMap std::map<std::string, void (ConfigServer::*)(fileVector)>

class ConfigServer;
class ConfigRequest;

typedef struct  s_error_page {
	std::vector<int>    error_codes; // all of the codes that will be redirected
	std::string         uri;		// uri to which they are redirected
}               t_error_page;

bool 			is_digits(const std::string &str);
unsigned int	str_to_ip(std::string strIp);
std::string		remove_adjacent_slashes(const std::string &str);

#endif