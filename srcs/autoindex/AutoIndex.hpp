#ifndef AUTO_INDEX_HPP
# define AUTO_INDEX_HPP

# include "webserv.hpp"

class AutoIndex
{
public:
    AutoIndex();
    AutoIndex(const AutoIndex& copy);
    virtual ~AutoIndex();

    const AutoIndex& operator=(const AutoIndex& obj);

    static std::string  get_page(const char* path, const std::string& host, int port);

private:
    static std::string  get_link(const std::string& dir_entry, const std::string& dir_name, const std::string& host, int port);
};

#endif /* AUTO_INDEX_HPP */