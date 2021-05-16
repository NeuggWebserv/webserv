#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "webserv.hpp"

class Config
{
public:
    void pull(const char *filename)
    {
        /* For test */
    }

    std::vector<t_listen> get_all_listens(void)
    {
        /* For test */
        std::vector<t_listen> all_listens;
        t_listen test_listen;

        test_listen.host = 0;
        test_listen.port = 8080;
        all_listens.push_back(test_listen);
        return all_listens;
    }
};

#endif