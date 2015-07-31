#ifndef IWEBSOCKETSERVER_H
#define IWEBSOCKETSERVER_H

#include <string>
#include <websocketpp\common\connection_hdl.hpp>

class IWebsocketServerListener;

class IWebsocketServer
{
public:
    virtual void run() = 0;
	virtual void sendMessage(websocketpp::connection_hdl hdl, const std::string& message) = 0;
    virtual void addListener(IWebsocketServerListener* listener) = 0;
    virtual void removeListener(IWebsocketServerListener* listener) = 0;
    virtual ~IWebsocketServer()
    {
    }
};

#endif // IWEBSOCKETSERVER_H

