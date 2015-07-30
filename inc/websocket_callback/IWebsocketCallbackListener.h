#ifndef IWEBSOCKETCALLBACKLISTENER_H
#define IWEBSOCKETCALLBACKLISTENER_H

struct libwebsocket;

class IWebsocketCallbackListener
{
public:
    virtual void onConnected(libwebsocket* wsi,  struct session_data* data) = 0;
    virtual void onMessageReceived(libwebsocket* wsi, const char* message) = 0;
    virtual void onDisconnected(libwebsocket* wsi) = 0;
};

#endif // IWEBSOCKETCALLBACKLISTENER_H

