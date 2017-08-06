#include "server.h"
//#include <thread>
//#include <iostream>
//#include <chrono>
//#include "random.h"



int main(int argc, char *argv[])
{
    const int SERVER_PORT = 8877;

    Server server;
    server.start(SERVER_PORT);



    return 0;
}
