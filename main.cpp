#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "usage : ./ircserv <port> <password>\n";
        return (0);
    }
    (void) argv;
    return (0);
}