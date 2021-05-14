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
#include "ConfigUtil.hpp"

# define DEFAULT_CONFIG "./files/error/error.conf"

class ConfigServer;
class ConfigRequest;
class Request;

class Config
{
public:
    //default Class component.
    Config(std::string path = DEFAULT_CONFIG);
	typedef std::vector<std::string> fileVector;
    Config(Config const &src);
    virtual ~Config(void);
    Config                    &operator=(Config const &co);

    int                       pull(const char *filename);
    std::vector<ConfigServer> get_server_list() const;
    std::vector<t_listen>     get_all_listens() const;
    ConfigRequest	          get_config_for_request(t_listen const address,\
					            std::string const uri, std::string host_name, const std::string& method,\
					            Request &request) const;
private:
    bool                      get_server_for_request(ConfigServer &ret, t_listen const address, std::string const host_name) const;
    std::vector<ConfigServer>   server_list;
};

std::ostream                        &operator<<(std::ostream &out, const Config &config);

#endif

