#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include "IChatServer.hpp"

#include "WebsocketServer/IWebsocketServerListener.hpp"

#include <memory>
#include <map>
#include <string>

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <websocketpp/common/connection_hdl.hpp>

#include <Model/User.hpp>

#include <JsonChatProtocol/json_request/LoginRequestJson.hpp>
#include <JsonChatProtocol/json_request/SendMessageJson.hpp>

class IWebsocketServer;
class IServerJsonParser;
class IServerJsonFactory;
class IUserDAO;



class ChatServer : public IChatServer, public IWebsocketServerListener
{

    using userConnectionMap = boost::bimap<int,
                                           boost::bimaps::set_of<connection_hdl,
                                                                 std::owner_less
                                                                 <connection_hdl> > >;
public:
    ChatServer(int port);

    // Implements IChatServer interface
public:
    void run();
    void close();
    // Implements IWebsocketServerListener interface
public:
    void onMessageReceived(connection_hdl hdl, const std::string& message);
    void onDisconnected(connection_hdl hdl);


private:
    bool isUserLoggedIn(int userId);
	void tryLogInUser(const LoginRequestJson&, connection_hdl hdl);
    void logInUser(const UserDetails& userDetails, connection_hdl hdl);

    int getUserId(connection_hdl hdl);
    connection_hdl getConnection(int userId);

    void handleGetContactsRequest(connection_hdl hdl);
	void handleSendMessage(const SendMessageJson& requestJson, connection_hdl hdl);
	void setContactsOnlineStatus(std::vector<Contact>& contacts);
    void notifyContactsOnOnlineStatusChanged(int userId, bool isOnline);
private:
    std::unique_ptr<IWebsocketServer> p_websocketServer;
    std::unique_ptr<IServerJsonParser> p_jsonParser;
    std::unique_ptr<IServerJsonFactory> p_jsonFactory;
    std::unique_ptr<IUserDAO> p_userDAO;

    userConnectionMap m_loggedClients;

};

#endif // CHATSERVER_HPP

