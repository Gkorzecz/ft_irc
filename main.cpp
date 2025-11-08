#include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: /ircserver [port]\n";
        return (0);
    }
    int PortNumber = std::atoi(argv[1]);
    Server srv(PortNumber);
    if (!srv.start())
    {
        std::cout << "server error\n";
        return (0);
    }
    // catch (const exception &e)
    return (0);
}