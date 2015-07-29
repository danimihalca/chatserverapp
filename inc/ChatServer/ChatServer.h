#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "IChatServer.h"

#include "WebsocketServer/IWebsocketServerListener.h"

#include <memory>
#include <map>
#include <string>

class IWebsocketServer;
class IJsonParser;
class JsonFactory;

struct libwebsocket;
struct UserPOD;

class ChatServer: public IChatServer, public IWebsocketServerListener
{
    struct UserValues
    {
        int dbId;
        libwebsocket* websocket;
    };

public:
    ChatServer(int port);

    //Implements IChatServer interface
public:
    void run();

    //Implements IWebsocketServerListener interface
public:
    void onMessageReceived(libwebsocket* websocket, const std::string& message);
    void onDisconnected(libwebsocket* websocket);


private:
    void login(UserPOD user, libwebsocket* websocket);

private:
    std::unique_ptr<IWebsocketServer> p_websocketServer;
    std::unique_ptr<IJsonParser> p_jsonParser;
    std::unique_ptr<JsonFactory> p_jsonFactory;
    std::map<std::string, UserValues> m_loggedClients;

};

#endif // CHATSERVER_H

