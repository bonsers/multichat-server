#include "server.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>



Server::Server()
{
}


void Server::start(const int PORT)
{
    Random r1;
    temp.resize(14);
    temp = getShuffledColors(r1);

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);   // host to network short
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  // host to network long

    int listenSock;
    if ((listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error: Could not create listen socket\n";
        //return 1;
    }
    std::cout << "Server starting..." << std::endl;

    if ((bind(listenSock, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) < 0)
    {
        std::cout << "Error: Could not bind socket\n";
        //return 1;
    }

    int waitSize = 16;
    if (listen(listenSock, waitSize) < 0)
    {
        std::cout << "Error: could not open socket for listening\n";
        //return 1;
    }
    std::cout << "Listening for connections..." << std::endl;

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = 0;

    while (1)
    {
        int newConnSock;
        for (int i = 0; i < 32; i++)
        {
            newConnSock = accept(listenSock, (struct sockaddr *)&clientAddress, &clientAddressLen);
            if (newConnSock == 0)
            {
                std::cout << "Error: Failed to accept client's connection\n";
            }
            else
            {
                std::cout << "~~ " << newConnSock << " client connected (ip address: " << inet_ntoa(clientAddress.sin_addr) << ")" << std::endl;
                connSock[i] = newConnSock;
                connCount++;
                t[i] = std::thread(&Server::clientHandler, this, i);    // create thread for each client
                t[i].detach();
            }
        }
    }
    //close(listenSock);
}


void Server::clientHandler(int i)
{
    int n = 0;
    while (connSock[i] > 0)
    {
        char buf[100];
        n = recv(connSock[i], buf, 100, 0);
        if (n == 0)
        {
            std::cout << "~~ " << connSock[i] << " client successfully disconnected" << std::endl;
            close(connSock[i]);
            break;
        }
        else if (n == -1)
        {
            std::cout << "Error: \n"; //
        }
        std::cout << connSock[i] << " " << buf <<  std::endl;
        memset(&buf[0], 0, sizeof(buf));
    }

    close(connSock[i]);
}


std::vector<int> Server::getShuffledColors(Random& r)
{
    std::vector<int> result = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};   // 14 ansi colors to choose from

    for (int i = 13; i > 0; --i)
    {
        int j = r.getRandomInt(0, 13);
        int temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }

    return result; // return shuffled array of ints
}
