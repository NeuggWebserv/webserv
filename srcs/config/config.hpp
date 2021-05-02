/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youlee <youlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:30:45 by youlee            #+#    #+#             */
/*   Updated: 2021/04/19 19:51:26 by youlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"

#include "ConfigRequest.hpp"
#include "ConfigServer.hpp"
#include "ConfigUtil.hpp"
#include "ConfigReader.hpp"

class Config
{
    public:
		//default Class component.
        Config(std::string Path = DEFAULT_CONFIG);
        Config(Config const &src);
        virtual	~Config(void);

        Config     					&operator=(Config const &co);
        int         				pull(const char *filename);
        std::vector<ConfigServer>	getServer_list() const;
		std::vector<t_listen>		getAllListens() const;
    private:
		bool						getServer_for_request(ConfigServer &ret, t_listen const address, std::string const host_name) const;
		std::vector<ConfigServer>	server_list;
};

std::ostream						&operator<<(std::ostream &out, const Config &config);

#endif
