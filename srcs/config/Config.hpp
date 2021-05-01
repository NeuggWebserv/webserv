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

#include "ConfigServer.hpp"
#include "ConfigUtil.hpp"
#include "ConfigReader.hpp"

class Config
{
    public:
		//default Class component.
        Config() {}
        Config(std::string Path);
        Config(Config const &src);
        virtual	~Config(void);

        Config     					&operator=(Config const &Co);
        int         				pull(const char *filename);
        std::vector<ConfigServer>	getServer_list() const;
        std::vector<t_listen> get_all_listens() const;

    private:
        
		std::vector<ConfigServer>	server_list;
};

std::ostream						&operator<<(std::ostream &out, const Config &config);

#endif
