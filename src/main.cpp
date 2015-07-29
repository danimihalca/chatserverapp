#include <memory>
#include <iostream>

#include <signal.h>

#include "ChatServer/ChatServer.h"

bool interrupted = false;

void  INThandler(int sig)
{
    std::cout << "Server interrupted" << std::endl;
    interrupted = true;
}

int main()
{
    signal(SIGINT, INThandler);
    std::unique_ptr<IChatServer> chatServer(new ChatServer(9003));

    while (!interrupted)
    {
        chatServer->run();
    }
    return 0;
}
