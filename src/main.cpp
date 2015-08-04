#include <memory>
#include <iostream>

#include <signal.h>

#include "ChatServer/ChatServer.hpp"


std::unique_ptr<IChatServer> chatServer;

void  INThandler(int sig)
{
    std::cout << "Server interrupted" << std::endl;
    chatServer->close();
}

int main()
{
    signal(SIGINT, INThandler);
    chatServer.reset(new ChatServer(9003));


    chatServer->run();

    return 0;
}
