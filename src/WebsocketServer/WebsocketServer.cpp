#include "WebsocketServer/WebsocketServer.h"
#include "WebsocketServer/IWebsocketServerListener.h"

#include <iostream>
#include <bitset>

#include <cstdlib>
#include <cstring>

#include "JsonProtocol/JsonParser.h"

WebsocketServer::WebsocketServer(int port) :
    m_connectedClients()
{
    m_protocols[0] = default_protocol;
    m_protocols[1] = null_protocol;

    int log_level = (1 << LLL_COUNT) -
                    1 -
                    LLL_PARSER -
                    LLL_DEBUG;

    lws_set_log_level(log_level,NULL);

    memset(&m_info, 0, sizeof m_info);

    m_info.port = port;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;
    m_info.http_proxy_address = NULL;
    m_info.user = static_cast<IWebsocketCallbackListener*>(this);

//    m_info.ka_interval =1;
//    m_info.ka_probes = 1;
//    m_info.ka_time=1;

    p_context = libwebsocket_create_context(&m_info);

    if (p_context == NULL)
    {
        std::cout << "libwebsocket init failed" << std::endl;
    }
}

WebsocketServer::~WebsocketServer()
{
    libwebsocket_context_destroy(p_context);
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
    libwebsocket_service(p_context, 50);
}

void WebsocketServer::sendMessage(libwebsocket* wsi, const std::string& message)
{
    size_t length = message.length();
    unsigned char* buffer_location =
        &(m_connectedClients[wsi]->buf[LWS_SEND_BUFFER_PRE_PADDING]);
    memcpy(buffer_location, message.c_str(), length);
    m_connectedClients[wsi]->len = length;
    libwebsocket_callback_on_writable(p_context, wsi);
}

void WebsocketServer::onConnected(libwebsocket* wsi, session_data* data)
{
    m_connectedClients[wsi] = data;
    std::cout << "Connected: " << m_connectedClients[wsi]->ip << std::endl;
}

void WebsocketServer::onMessageReceived(libwebsocket* wsi, const char* message)
{
    for(auto listener: m_listeners)
    {
        listener->onMessageReceived(wsi, message);
    }
}

void WebsocketServer::onDisconnected(libwebsocket* wsi)
{
    std::cout << "Disconnected: " << m_connectedClients[wsi]->ip << std::endl;
    for(auto listener: m_listeners)
    {
        listener->onDisconnected(wsi);
    }
    m_connectedClients.erase(wsi);
}
