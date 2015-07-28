#include "WebsocketServer/WebsocketServer.h"

#include <iostream>
#include <bitset>

#include <cstdlib>
#include <cstring>

WebsocketServer::WebsocketServer(int port) :
    m_connectedClients()
{
    m_protocols[0] = default_protocol;
    m_protocols[1] = null_protocol;

    int log_level = (1 << LLL_COUNT) -
                    1 -
                    LLL_PARSER -
                    LLL_DEBUG;

//    std::cout << "LOG LEVEL: " << std::bitset<16>(log_level) << std::endl;

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
        fprintf(stderr, "libwebsocket init failed\n");
    }
}

WebsocketServer::~WebsocketServer()
{
    std::cout << "WebsocketServer::~WebsocketServer" << std::endl;
    libwebsocket_context_destroy(p_context);
}

void WebsocketServer::run()
{
    libwebsocket_service(p_context, 50);
}

void WebsocketServer::sendMessage(libwebsocket* wsi, const char* message)
{
    size_t length = strlen(message);
    unsigned char* buffer_location =
        &(m_connectedClients[wsi]->buf[LWS_SEND_BUFFER_PRE_PADDING]);
    memcpy(buffer_location, message, length);
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
    std::cout << "Message received: \"" << message << "\" from: " <<
    m_connectedClients[wsi]->ip << std::endl;
    sendMessage(wsi, message);
}

void WebsocketServer::onDisconnected(libwebsocket* wsi)
{
    std::cout << "Disconnected: " << m_connectedClients[wsi]->ip << std::endl;
    m_connectedClients.erase(wsi);
}

const std::string WebsocketServer::getIpFromWebsocket(libwebsocket* wsi)
{
    char name[40];
    char ip[25];
    int fd = libwebsocket_get_socket_fd(wsi);
    libwebsockets_get_peer_addresses(p_context,wsi,fd,name,40,ip,25);

    std::string address(ip);
    return address;
}
