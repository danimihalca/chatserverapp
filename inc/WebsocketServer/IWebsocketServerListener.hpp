#ifndef IWEBSOCKETSERVERLISTENER_HPP
#define IWEBSOCKETSERVERLISTENER_HPP

#include <string>

#include <websocketpp/common/connection_hdl.hpp>

using websocketpp::connection_hdl;


class IWebsocketServerListener
{
public:
    virtual void onMessageReceived(connection_hdl     hdl,
                                   const std::string& message) = 0;
    virtual void onDisconnected(connection_hdl hdl) = 0;

    virtual ~IWebsocketServerListener()
    {
    }
};

#endif // IWEBSOCKETSERVERLISTENER_HPP

