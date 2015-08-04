#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP

#include "IWebsocketServer.hpp"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <map>
#include <list>
#include <string>
#include <set>

using asioServer = websocketpp::server<websocketpp::config::asio>;

class IWebsocketServerListener;

class WebsocketServer : public IWebsocketServer
{
public:
    WebsocketServer(int port);
    ~WebsocketServer();

    // Implements IWebsocketServer interface
public:
    void run();
    void close();
    void sendMessage(connection_hdl hdl, const std::string& message);
    void addListener(IWebsocketServerListener* listener);
    void removeListener(IWebsocketServerListener* listener);

private:
    void onConnected(connection_hdl hdl);
    void onMessageReceived(connection_hdl hdl, asioServer::message_ptr message);
    void onDisconnected(connection_hdl hdl);

private:
    bool b_isClosing;
    int m_port;
    std::list<IWebsocketServerListener*> m_listeners;
    std::set<connection_hdl,std::owner_less<connection_hdl>> m_connections;
    asioServer m_server;
};

#endif // WEBSOCKETSERVER_HPP

