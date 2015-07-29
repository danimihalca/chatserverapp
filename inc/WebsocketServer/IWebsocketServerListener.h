#ifndef IWEBSOCKETSERVERLISTENER_H
#define IWEBSOCKETSERVERLISTENER_H

#include <string>

struct libwebsocket;

class IWebsocketServerListener
{
public:
    virtual ~IWebsocketServerListener()
    {

    }

    virtual void onMessageReceived(libwebsocket* websocket, const std::string& message) = 0;
    virtual void onDisconnected(libwebsocket* websocket) = 0;
};

#endif // IWEBSOCKETSERVERLISTENER_H

