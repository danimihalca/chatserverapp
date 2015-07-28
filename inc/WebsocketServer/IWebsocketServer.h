#ifndef IWEBSOCKETSERVER_H
#define IWEBSOCKETSERVER_H

class libwebsocket;

class IWebsocketServer
{
public:
    virtual void run() = 0;
    virtual void sendMessage(libwebsocket* wsi, const char* message) = 0;
    virtual ~IWebsocketServer()
    {
    }
};

#endif // IWEBSOCKETSERVER_H

