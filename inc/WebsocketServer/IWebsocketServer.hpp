#ifndef IWEBSOCKETSERVER_HPP
#define IWEBSOCKETSERVER_HPP

#include <string>

#include <websocketpp/common/connection_hdl.hpp>

using websocketpp::connection_hdl;

class IWebsocketServerListener;

class IWebsocketServer
{
public:
    virtual void run() = 0;
    virtual void close() = 0;
    virtual void sendMessage(connection_hdl     hdl,
                             const std::string& message) = 0;
    virtual void addListener(IWebsocketServerListener* listener) = 0;
    virtual void removeListener(IWebsocketServerListener* listener) = 0;
    virtual ~IWebsocketServer()
    {
    }
};

#endif // IWEBSOCKETSERVER_HPP

