#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "IChatServer.h"

#include "WebsocketServer/IWebsocketServerListener.h"

#include <memory>
#include <map>
#include <string>

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <websocketpp/common/connection_hdl.hpp>

class IWebsocketServer;
class IJsonParser;
class JsonFactory;

class UserCredentials;

class ChatServer : public IChatServer, public IWebsocketServerListener
{

public:
    ChatServer(int port);

    //Implements IChatServer interface
public:
    void run();

    //Implements IWebsocketServerListener interface
public:
    void onMessageReceived(websocketpp::connection_hdl hdl,
                           const std::string&          message);
    void onDisconnected(websocketpp::connection_hdl hdl);


private:
    void login(const UserCredentials&      userCredentials,
               websocketpp::connection_hdl hdl);

private:
    std::unique_ptr<IWebsocketServer> p_websocketServer;
    std::unique_ptr<IJsonParser> p_jsonParser;
    std::unique_ptr<JsonFactory> p_jsonFactory;
    boost::bimap<int,
                 boost::bimaps::set_of<websocketpp::connection_hdl,
                                       std::owner_less<websocketpp::
                                                       connection_hdl> > >
    m_loggedClients;

};

#endif // CHATSERVER_H

