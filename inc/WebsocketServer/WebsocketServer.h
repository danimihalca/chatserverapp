#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "IWebsocketServer.h"
#include "websocket_callback/IWebsocketCallbackListener.h"
#include "websocket_callback/websocket_callback.h"

#include <map>
#include <list>
#include <string>


class IWebsocketServerListener;

class WebsocketServer :
    public IWebsocketServer,
    public IWebsocketCallbackListener
{
public:
    WebsocketServer(int port);
    ~WebsocketServer();

    //Implements IWebsocketServer interface
public:
    void run();
    void sendMessage(libwebsocket* wsi, const std::string& message);
    void addListener(IWebsocketServerListener* listener);
    void removeListener(IWebsocketServerListener* listener);

    //Implements IWebsocketCallbackListener interface
public:
    void onConnected(libwebsocket* wsi, struct session_data* data);
    void onMessageReceived(libwebsocket* wsi, const char* message);
    void onDisconnected(libwebsocket* wsi);

private:
    struct libwebsocket_context* p_context;
    struct libwebsocket_protocols m_protocols[2];
    struct lws_context_creation_info m_info;

    std::list<IWebsocketServerListener*> m_listeners;

    std::map<libwebsocket*, struct session_data*> m_connectedClients;
};

#endif // WEBSOCKETSERVER_H

