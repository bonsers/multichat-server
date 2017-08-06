#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <vector>

#include "random.h"
#include "ansi.h"

class Server
{
public:
    Server();
    void start(const int PORT);
private:
    void clientHandler(int i);
    std::thread t[32];
    int connSock[32];
    int connCount = 0;
    std::vector<int> getShuffledColors(Random& r);
    std::vector<int> temp;
};

#endif // SERVER_H
