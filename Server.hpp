#pragma once

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>

class Server
{
    private :
        static const int MAX_CLIENTS = 10;
        static const int MAX_MSG_LENGHT = 512;
        int             _port;
        int             _Socket_fd;
        int             _clients[MAX_CLIENTS];

    public :
        Server(int port);
        ~Server();
        bool start(void);
        
    private :
        bool    createListeningSocket(void);
        bool    eventLoop(void);
        int     buildPollfds(struct pollfd *pfds, int *map);
        void    acceptClient(void);
        void    relay(int idx);
};
