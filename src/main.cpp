#include <memory>
#include <iostream>

#include <signal.h>

#include "WebsocketServer/WebsocketServer.h"

bool interrupted = false;

void  INThandler(int sig)
{
    std::cout << "Server interrupted" << std::endl;
    interrupted = true;
}

int main()
{
    signal(SIGINT, INThandler);
    std::unique_ptr<IWebsocketServer> websocketServer(new WebsocketServer(9003));

    while (!interrupted)
    {
        websocketServer->run();
    }
    return 0;
}
