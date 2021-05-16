#include "AutoIndex.hpp"

AutoIndex::AutoIndex() {}

AutoIndex::AutoIndex(const AutoIndex& copy)
{
    *this = copy;
}

AutoIndex::~AutoIndex()
{
    /* Nothing allocated dynamically */
}

const AutoIndex& AutoIndex::operator=(const AutoIndex& obj)
{
    (void)obj;
    return *this;
}

std::string AutoIndex::get_page(const char* path, const std::string& host, int port)
{
    std::string dir_name(path);
    DIR *dir = opendir(path);
    std::string page = "<!DOCTYPE html>\n\
                        <html>\n\
                        <head>\n\
                        <title>" + dir_name + "</title>\n\
                        </head>\n\
                        <body>\n\
                        <h1>INDEX</h1>\n\
                        <p>\n";

    if (dir == NULL)
    {
        std::cerr << "Error: could not open [" << path << "]" << std::endl;
        return "";
    }
    if (dir_name[0] != '/')
        dir_name = "/" + dir_name;
    for (struct dirent *dir_entry = readdir(dir); dir_entry; dir_entry = readdir(dir))
    {
        page += AutoIndex::get_link(std::string(dir_entry->d_name), dir_name, host, port);
    }
    page +="</p>\n\
            </body>\n\
            </html>\n";
    closedir(dir);
    return page;
}

std::string AutoIndex::get_link(const std::string& dir_entry, const  std::string& dir_name, const std::string& host, int port)
{
    std::stringstream   ss;
    ss << "\t\t<p><a href=\"http://" + host + ":" << \
        port << dir_name + "/" + dir_entry + "\">" + dir_entry + "</a></p>\n";
    return ss.str();
}