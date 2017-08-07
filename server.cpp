#include "server.h"
#include <string.h>
#include <unistd.h>
#include <iostream>



Server::Server()
{
}


Server::~Server()
{
    close(m_listenSock);
}


void Server::start(const int PORT)
{
    Random r1;
    //m_fgColors.resize(13);
    m_fgColors = getShuffledColors(r1);

    //for (int j = 0; j < m_fgColors.size(); j++)
    //{
    //    std::cout << m_fgColors[j] << "HELLO WORLD!" << ansi::DEFAULT << std::endl;
    //}

    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_port = htons(PORT);   // host to network short
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  // host to network long

    if ((m_listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error: Could not create listen socket\n";
        //return 1;
    }
    std::cout << "Server starting..." << std::endl;

    if ((bind(m_listenSock, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress))) < 0)
    {
        std::cout << "Error: Could not bind socket\n";
        //return 1;
    }

    if (listen(m_listenSock, m_waitSize) < 0)
    {
        std::cout << "Error: could not open socket for listening\n";
        //return 1;
    }
    std::cout << "Listening for connections..." << std::endl;

    while (1)
    {
        int newConnSock;
        for (int i = 0; i < 13; i++)
        {
            newConnSock = accept(m_listenSock, (struct sockaddr *)&m_clientAddress, &m_clientAddressLen);
            if (newConnSock == 0)
            {
                std::cout << "Error: Failed to accept client's connection\n";
            }
            else
            {
                std::cout << ansi::LIGHTGRAY << "~~ " << newConnSock << " client connected (ip address: " << inet_ntoa(m_clientAddress.sin_addr) << ")" << std::endl;
                m_connSock[i] = newConnSock;
                m_connCount++;
                m_thread[i] = std::thread(&Server::clientHandler, this, i);    // create thread for each client
                m_thread[i].detach();
            }
        }
    }
}


void Server::clientHandler(int i)
{
    int n = 0;
    while (m_connSock[i] > 0)
    {
        char buf[100];
        n = recv(m_connSock[i], buf, 100, 0);
        if (n == 0)
        {
            std::cout << ansi::LIGHTGRAY << "~~ " << m_connSock[i] << " client successfully disconnected" << ansi::DEFAULT << std::endl;
            close(m_connSock[i]);
            break;
        }
        else if (n == -1)
        {
            std::cout << "Error: \n"; //
        }
        std::cout << m_fgColors[i] << m_connSock[i] << " " << buf << ansi::DEFAULT << std::endl;
        memset(&buf[0], 0, sizeof(buf));
    }

    close(m_connSock[i]);
}


std::vector<std::string> Server::getShuffledColors(Random& r)
{
    //std::vector<int> result = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};   // 14 ansi colors to choose from
    std::vector<std::string> result = {ansi::RED, ansi::GREEN, ansi::YELLOW, ansi::BLUE, ansi::MAGENTA, ansi::CYAN, ansi::LIGHTGRAY, ansi::LIGHTRED, ansi::LIGHTBLUE, ansi::LIGHTGREEN, ansi::LIGHTYELLOW, ansi::LIGHTMAGENTA, ansi::LIGHTCYAN};    // 13 ansi colors to choose from

    for (int i = 12; i > 0; --i)
    {
        int j = r.getRandomInt(0, 12);
        std::string temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }

    return result; // return shuffled array of strings
}
