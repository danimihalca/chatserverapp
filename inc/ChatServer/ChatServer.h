#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "IChatServer.h"
#include <memory>

class IWebsocketServer;

class ChatServer: public IChatServer
{
public:
    ChatServer(int port);

    //Implements IChatServer
public:
    void run();


private:
    std::unique_ptr<IWebsocketServer> p_websocketServer;
};

#endif // CHATSERVER_H

