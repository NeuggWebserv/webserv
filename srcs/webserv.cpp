#include "webserv.hpp"
#include "server.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        Server server;
        
        try
        {
            server.set_config(argv[1]);
            server.set_server();
            server.run();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "ERROR: Invalid number of arguments" << std::endl;
        return 1;
    }
    return 0;
}