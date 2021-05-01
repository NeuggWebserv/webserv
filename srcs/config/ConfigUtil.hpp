/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtil.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:50:59 by youlee            #+#    #+#             */
/*   Updated: 2021/04/23 19:47:14 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGUTIL_HPP
#define CONFIGUTIL_HPP

# include "webserv.hpp"

class ConfigServer;
class ConfigRequest;

bool 			is_digits(const std::string &str);
unsigned int	str_to_ip(std::string str_ip);
std::string		remove_adjacent_slashes(const std::string &str);

#endif
