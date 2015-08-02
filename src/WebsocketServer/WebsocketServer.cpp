#include "WebsocketServer/WebsocketServer.hpp"
#include "WebsocketServer/IWebsocketServerListener.hpp"

#include <iostream>
#include <bitset>
#include <string>

#include <cstdlib>
#include <cstring>

#include "JsonProtocol/ServerJsonParser.hpp"

#include <debug_utils/log_debug.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WebsocketServer::WebsocketServer(int port) :
    m_port(port),
    b_isClosing(false)
{
    m_server.init_asio();

    m_server.set_open_handler(bind(&WebsocketServer::onConnected, this, ::_1));
    m_server.set_close_handler(bind(&WebsocketServer::onDisconnected,
                                    this,
                                    ::_1));
    m_server.set_message_handler(bind(&WebsocketServer::onMessageReceived, this,
                                      ::_1, ::_2));
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
    m_server.listen(m_port);
    m_server.start_accept();
    m_server.run();
}

void WebsocketServer::sendMessage(connection_hdl     hdl,
                                  const std::string& message)
{
    m_server.send(hdl, message, websocketpp::frame::opcode::TEXT);
}

void WebsocketServer::onConnected(connection_hdl hdl)
{
    asioServer::connection_ptr c = m_server.get_con_from_hdl(hdl);
    log_debug("CONNECTED: %s\n", c->get_remote_endpoint().c_str());
}

void WebsocketServer::onMessageReceived(
    connection_hdl          hdl,
    asioServer::message_ptr message)
{
    const std::string m = message->get_payload();
    for(auto listener: m_listeners)
    {
        listener->onMessageReceived(hdl, m);
    }
}

void WebsocketServer::onDisconnected(connection_hdl hdl)
{
    asioServer::connection_ptr c = m_server.get_con_from_hdl(hdl);
    log_debug("DISCONNECTED: %s\n",c->get_remote_endpoint().c_str());
    if (!b_isClosing)
    {
        for (auto listener : m_listeners)
        {
            listener->onDisconnected(hdl);
        }
    }
}
