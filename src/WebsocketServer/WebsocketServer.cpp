#include "WebsocketServer/WebsocketServer.h"
#include "WebsocketServer/IWebsocketServerListener.h"

#include <iostream>
#include <bitset>

#include <cstdlib>
#include <cstring>

#include "JsonProtocol/JsonParser.h"

WebsocketServer::WebsocketServer(int port) :
    b_isClosing(false)
{
}

WebsocketServer::~WebsocketServer()
{
    b_isClosing = true;
}

void WebsocketServer::addListener(IWebsocketServerListener* listener)
{
    m_listeners.push_back(listener);
}

void WebsocketServer::removeListener(IWebsocketServerListener* listener)
{
    m_listeners.remove(listener);
}

void WebsocketServer::run()
{
}

void WebsocketServer::sendMessage(websocketpp::connection_hdl hdl, const std::string& message)
{

}

void WebsocketServer::onConnected(websocketpp::connection_hdl hdl)
{
    std::cout << "Connected: " << "<IP>" << std::endl;
}

void WebsocketServer::onMessageReceived(websocketpp::connection_hdl hdl, const char* message)
{
    for(auto listener: m_listeners)
    {
        listener->onMessageReceived(hdl, message);
    }
}

void WebsocketServer::onDisconnected(websocketpp::connection_hdl hdl)
{
	if (!b_isClosing)
	{
	}
}
