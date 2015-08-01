#include "ChatServer/ChatServer.h"

#include "WebsocketServer/WebsocketServer.h"

#include "JsonProtocol/JsonParser.h"
#include "JsonProtocol/JsonFactory.h"

#include "DAO/UserDAO.h"


ChatServer::ChatServer(int port) :
    p_websocketServer(new WebsocketServer(port)),
    p_jsonParser(new JsonParser()),
    p_jsonFactory(new JsonFactory())
{
    p_websocketServer->addListener(this);
}

void ChatServer::run()
{
    p_websocketServer->run();
}

void ChatServer::onMessageReceived(websocketpp::connection_hdl hdl,
                                   const std::string&          message)
{
//    try
//    {
    p_jsonParser->parseJsonString(message);
//    }
//    catch(Json::Exception& e)
//    {
//       TODO: send invalid JSON response message
//       return;
//    }
    ChatClient_Action_Type actionType = p_jsonParser->getActionType();
    switch(actionType)
    {
        case CC_LOGIN_REQUEST:
        {
            UserCredentials userCredentials = p_jsonParser->getUser();
            login(userCredentials, hdl);
            break;
        }

        default:
        {
            break;
        }
    }
}

void ChatServer::onDisconnected(websocketpp::connection_hdl hdl)
{
//    std::string username;
//    for (auto client = m_loggedClients.begin();
//         client != m_loggedClients.end();
//         ++client)
//    {
//        if (client->right.lock().get() == hdl1.lock().get())
//        {
////            username = client->first;
//            std::cout << "Disconnected user: " << username << std::endl;
//            m_loggedClients.erase(client);
//            break;
//        }
//    }

    auto user = m_loggedClients.right.find(hdl);
    if (user != m_loggedClients.right.end())
    {
        std::cout << "Disconnected user: " << user->second << std::endl;
        m_loggedClients.right.erase(user);
    }
    else
    {
        std::cout << "Disconnected user not found!" << std::endl;
    }

}

void ChatServer::login(const UserCredentials&      userCredentials,
                       websocketpp::connection_hdl hdl)
{
    UserDAO userDao;
//    m_loggedClients.by


    if (!userDao.isValidUser(userCredentials))
    {
        std::cout << "FAILED LOGIN" << std::endl;
        std::string authFailedJson = p_jsonFactory->createLoginFailedJSON(
            AUTH_FAILED);
        p_websocketServer->sendMessage(hdl,authFailedJson);
        return;
    }

    int id;
    if (m_loggedClients.left.find(id) != m_loggedClients.left.end())
    {
        std::string authFailedJson = p_jsonFactory->createLoginFailedJSON(
            AUTH_ALREADY_LOGGED_IN);
        p_websocketServer->sendMessage(hdl,authFailedJson);
        return;
    }

    std::cout << "Logged in user: " << userCredentials.getUserName() <<
    std::endl;
    UserDetails userDetails = userDao.getUserDetails(
        userCredentials.getUserName());
    m_loggedClients.insert(boost::bimap<int,
                                        boost::bimaps::set_of<websocketpp::
                                                              connection_hdl,
                                                              std::owner_less<
                                                                  websocketpp::
                                                                  connection_hdl> > >::value_type(
                               userDetails.getId(), hdl));
    std::string authFailedJson = p_jsonFactory->createLoginSuccessfulJSON(
        userDetails);
    p_websocketServer->sendMessage(hdl,authFailedJson);
}
