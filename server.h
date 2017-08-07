#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <vector>
#include <arpa/inet.h>
#include "random.h"
#include "ansi.h"

class Server
{
public:
    Server();
    ~Server();
    void start(const int PORT);
private:
    void clientHandler(int i);
    std::thread m_thread[13];
    int m_connSock[13];
    int m_connCount = 0;
    std::vector<std::string> getShuffledColors(Random& r);
    std::vector<std::string> m_fgColors;
    int m_waitSize = 13;
    int m_listenSock;
    sockaddr_in m_serverAddress;
    sockaddr_in m_clientAddress;
    socklen_t m_clientAddressLen = 0;
};

#endif // SERVER_H
