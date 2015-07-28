#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "IWebsocketServer.h"
#include "websocket_callback/IWebsocketCallbackListener.h"
#include "websocket_callback/websocket_callback.h"

#include <map>
#include <string>

class WebsocketServer :
    public IWebsocketServer,
    public IWebsocketCallbackListener
{
public:
    WebsocketServer(int port);
    ~WebsocketServer();

    //Implements IWebsocketServer
public:
    void run();
    void sendMessage(libwebsocket* wsi, const char* message);

    //Implements IWebsocketCallbackListener
public:
    void onConnected(libwebsocket* wsi, struct session_data* data);
    void onMessageReceived(libwebsocket* wsi, const char* message);
    void onDisconnected(libwebsocket* wsi);

private:
    const std::string getIpFromWebsocket(libwebsocket* wsi);

private:
    struct libwebsocket_context* p_context;
    struct libwebsocket_protocols m_protocols[2];
    struct lws_context_creation_info m_info;

    std::map<libwebsocket*, struct session_data*> m_connectedClients;
};

#endif // WEBSOCKETSERVER_H

