#ifndef IWEBSOCKETSERVER_H
#define IWEBSOCKETSERVER_H

#include <string>

struct libwebsocket;
class IWebsocketServerListener;

class IWebsocketServer
{
public:
    virtual void run() = 0;
    virtual void sendMessage(libwebsocket* wsi, const std::string& message) = 0;
    virtual void addListener(IWebsocketServerListener* listener) = 0;
    virtual void removeListener(IWebsocketServerListener* listener) = 0;
    virtual ~IWebsocketServer()
    {
    }
};

#endif // IWEBSOCKETSERVER_H

