/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:50:48 by youlee            #+#    #+#             */
/*   Updated: 2021/04/23 20:02:10 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include "webserv.hpp"

#define BUFFER_SIZE 2048

class ConfigReader
{
	public:
		ConfigReader();
		ConfigReader(ConfigReader const &Co);
		virtual ~ConfigReader();
		
		ConfigReader		&operator=(ConfigReader const &Co);
		static fileVector	readFile(const char *file);
		static fileVector	split(std::string str, std::string charset);
		class FileNotFoundException: public std::exception{
			virtual const char *what() const throw();
		};
	private:
};

#endif
