#ifndef IWEBSOCKETSERVERLISTENER_H
#define IWEBSOCKETSERVERLISTENER_H

#include <string>

#include <websocketpp\common\connection_hdl.hpp>

class IWebsocketServerListener
{
public:
    virtual ~IWebsocketServerListener()
    {

    }

	virtual void onMessageReceived(websocketpp::connection_hdl hdl, const std::string& message) = 0;
	virtual void onDisconnected(websocketpp::connection_hdl hdl) = 0;
};

#endif // IWEBSOCKETSERVERLISTENER_H

