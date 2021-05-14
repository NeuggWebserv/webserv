/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathisfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 18:16:35 by youlee            #+#    #+#             */
/*   Updated: 2021/05/02 18:16:38 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int		path_is_file(const std::string &path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR) return (0);
		else if (s.st_mode & S_IFREG) return (1);
		else return (0);
	}
	else return (0);
}
