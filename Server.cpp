#include "Server.hpp"

/* Initialize all clients/socket fd's to -1 (not oppened)*/
Server::Server(int port) : _port(port), _Socket_fd(-1)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
        _clients[i] = -1;
}

/* Clear all fd's opened during server life.*/
Server::~Server()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (_clients[i] != -1)
            close(_clients[i]);
        if (_Socket_fd != -1)
            close(_Socket_fd);
    } 
}

bool Server::start(void)
{
    return (createListeningSocket() && eventLoop());
}

/* Basic socket initialisation :
    - Create a socket (which is a fd) with socket().
    - Bind the socket to an address and port using bind().
    - Listen for incoming connections with listen().
- socket() : AF_INET : IPv4 protocol | SOCK_STREAM: TCP socket */
bool Server::createListeningSocket(void)
{
    _Socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_Socket_fd == -1)
        return (false);
    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_port);
    if (bind(_Socket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        return (false);
    if (listen(_Socket_fd, MAX_CLIENTS) == -1)
        return (false);
    return (true);
}

/* int poll(struct pollfd fds[], nfds_t nfds, int timeout);
Take an array of fd's, number of fd's, timeout = -1 for infinite wait.
Check for POLLIN "events"
if it's from the listening socket -> a new client is trying to connect, accept it.
if it's from a client -> it's trying to send data, so relay it.*/
bool Server::eventLoop(void)
{
    struct pollfd pfds[MAX_CLIENTS + 1];
    int map[MAX_CLIENTS + 1];
    while (true)
    {
        int n = buildPollfds(pfds, map);
        if (poll(pfds, n, -1) <= 0)
            continue;
        for (int i = 0; i < n; i++)
        {
            if (!(pfds[i].revents & POLLIN))
                continue;
            if (map[i] == -1)
                acceptClient();
            else
                relay(map[i]);
        }
    }
    return (true);
}

/* Construct the structure pollfd and map that poll() will take as argument.
*/
int Server::buildPollfds(struct pollfd *pfds, int *map)
{
    int idx = 0;
    pfds[idx].fd = _Socket_fd;
    pfds[idx].events = POLLIN;
    map[idx] = -1;
    idx++;
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (_clients[i] != -1)
        {
            pfds[idx].fd = _clients[i];
            pfds[idx].events = POLLIN;
            map[idx] = i;
            idx++;
        }
    }
    return (idx);
}

/* To accept a new client, loops through the list and find the first
client slot not used yet.
if no slot is found close the fd directly.*/
void Server::acceptClient()
{
    int fd = accept(_Socket_fd, NULL, NULL);
    if (fd == -1)
        return;
    for (int idx = 0; idx < MAX_CLIENTS; idx++)
    {
        if (_clients[idx] == -1)
        {   
            _clients[idx] = fd;
            std::cout << "New client connected at slot : " << idx << std::endl;
            return;
        }
    }
    close(fd);
}

/* Transmit to every client what's send to the server.
Uncomment to supress echo. (client sending to himself, for testing now)
Also close clients if it can't recieve anything (disconnected).*/
void Server::relay(int idx)
{
    char buffer[MAX_MSG_LENGHT];
    ssize_t n = recv(_clients[idx], buffer, sizeof(buffer), 0);
    if (n <= 0)
    {   
        close(_clients[idx]); 
        _clients[idx] = -1;
        std::cout << "Client disconnected at slot : " << idx << std::endl;
        return ;
    }
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        // if (_clients[i] != -1 && i != idx) 
            send(_clients[i], buffer, n, 0);
    }
}