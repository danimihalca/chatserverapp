#include "ChatServer/ChatServer.h"

#include "WebsocketServer/WebsocketServer.h"

ChatServer::ChatServer(int port):
    p_websocketServer(new WebsocketServer(port))
{

}

void ChatServer::run()
{
    p_websocketServer->run();
}
