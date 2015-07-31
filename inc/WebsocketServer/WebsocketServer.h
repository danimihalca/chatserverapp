#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "IWebsocketServer.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

#include <map>
#include <list>
#include <string>


class IWebsocketServerListener;

class WebsocketServer :
    public IWebsocketServer
{
public:
    WebsocketServer(int port);
    ~WebsocketServer();

    //Implements IWebsocketServer interface
public:
    void run();
	void sendMessage(connection_hdl hdl, const std::string& message);
    void addListener(IWebsocketServerListener* listener);
    void removeListener(IWebsocketServerListener* listener);

public:
	void onConnected(connection_hdl hdl);
	void onMessageReceived(connection_hdl hdl, const char* message);
	void onDisconnected(connection_hdl hdl);

private:
	bool b_isClosing;

    std::list<IWebsocketServerListener*> m_listeners;
	server m_server;
};

#endif // WEBSOCKETSERVER_H

