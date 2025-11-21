#include "Server.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cout << "usage: /ircserver [port]\n";
            return (1);
        }
        int PortNumber = std::atoi(argv[1]);
        Server srv(PortNumber);
        srv.start();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return (1);
    }
}
