#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "IChatServer.h"

#include "WebsocketServer/IWebsocketServerListener.h"

#include <memory>
#include <map>
#include <string>

#include <websocketpp/common/connection_hdl.hpp>

class IWebsocketServer;
class IJsonParser;
class JsonFactory;

struct UserPOD;

class ChatServer: public IChatServer, public IWebsocketServerListener
{
    struct UserValues
    {
        int dbId;
		websocketpp::connection_hdl hdl;
    };

public:
    ChatServer(int port);

    //Implements IChatServer interface
public:
    void run();

    //Implements IWebsocketServerListener interface
public:
	void onMessageReceived(websocketpp::connection_hdl hdl, const std::string& message);
	void onDisconnected(websocketpp::connection_hdl hdl);


private:
	void login(UserPOD user, websocketpp::connection_hdl hdl);

private:
    std::unique_ptr<IWebsocketServer> p_websocketServer;
    std::unique_ptr<IJsonParser> p_jsonParser;
    std::unique_ptr<JsonFactory> p_jsonFactory;
    std::map<std::string, UserValues> m_loggedClients;

};

#endif // CHATSERVER_H

