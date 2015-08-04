#include "ChatServer/ChatServer.hpp"

#include "WebsocketServer/WebsocketServer.hpp"

#include "JsonProtocol/ServerJsonParser.hpp"
#include "JsonProtocol/ServerJsonFactory.hpp"

#include "DAO/UserDAO.hpp"

#include <debug_utils/log_debug.hpp>

ChatServer::ChatServer(int port) :
    p_websocketServer(new WebsocketServer(port)),
    p_jsonParser(new ServerJsonParser()),
    p_jsonFactory(new ServerJsonFactory()),
    p_userDAO(new UserDAO())
{
    p_websocketServer->addListener(this);
}

void ChatServer::run()
{
    p_websocketServer->run();
}

void ChatServer::close()
{
    p_websocketServer->close();
}

void ChatServer::onMessageReceived(connection_hdl     hdl,
                                   const std::string& message)
{

    if (!p_jsonParser->parseJsonString(message))
    {
        LOG_DEBUG("Received message: %s is invalid\n", message.c_str());
        return;
    }

    Chat_Action_Type actionType = p_jsonParser->getActionType();
    switch(actionType)
    {
        case LOGIN_REQUEST:
        {
            UserCredentials userCredentials =
                p_jsonParser->getUserCredentials();
            tryLogInUser(userCredentials, hdl);
            break;
        }

        case GET_CONTACTS_REQUEST:
        {
            handleGetContactsRequest(hdl);
            break;
        }

        default:
        {
            break;
        }
    }
}

void ChatServer::onDisconnected(connection_hdl hdl)
{
    auto user = m_loggedClients.right.find(hdl);
    if (user != m_loggedClients.right.end())
    {
        LOG_DEBUG("Disconnected user with id: %d\n", user->second);
        m_loggedClients.right.erase(user);
    }
}

bool ChatServer::isUserLoggedIn(int userId)
{
    return (m_loggedClients.left.find(userId) != m_loggedClients.left.end());
}

void ChatServer::tryLogInUser(const UserCredentials& userCredentials,
                              connection_hdl         hdl)
{
    std::string jsonResponse;

    if (!p_userDAO->isValidUser(userCredentials))
    {
        jsonResponse = p_jsonFactory->createLoginFailedJsonString(
            AUTH_INVALID_CREDENTIALS);
        p_websocketServer->sendMessage(hdl,jsonResponse);
        return;
    }

    UserDetails userDetails = p_userDAO->getUserDetails(
        userCredentials.getUserName());


    if (isUserLoggedIn(userDetails.getId()))
    {
        jsonResponse = p_jsonFactory->createLoginFailedJsonString(
            AUTH_ALREADY_LOGGED_IN);
        p_websocketServer->sendMessage(hdl,jsonResponse);
        return;
    }

    logInUser(userDetails,hdl);

    jsonResponse = p_jsonFactory->createLoginSuccessfulJsonString(userDetails);
    p_websocketServer->sendMessage(hdl,jsonResponse);
}

void ChatServer::logInUser(const UserDetails& userDetails, connection_hdl hdl)
{
    int userId = userDetails.getId();
    m_loggedClients.insert(userConnectionMap::value_type(userId, hdl));

    LOG_DEBUG("Logged in user with id: %d\n", userId);
}

int ChatServer::getUserId(connection_hdl hdl)
{
    auto user = m_loggedClients.right.find(hdl);
    if (user != m_loggedClients.right.end())
    {
        return user->second;
    }
    return -1;
}

void ChatServer::handleGetContactsRequest(connection_hdl hdl)
{
    LOG_DEBUG_METHOD;
    int userId = getUserId(hdl);
    Contacts contacts = p_userDAO->getContacts(userId);

    setContactsOnlineStatus(contacts);
    std::string contactsJsonResponse =
        p_jsonFactory->createGetContactsResponseJsonString(contacts);

    p_websocketServer->sendMessage(hdl, contactsJsonResponse);
}

void ChatServer::setContactsOnlineStatus(Contacts& contacts)
{
    for(Contact& contact: contacts)
    {
        bool isLoggedIn = isUserLoggedIn(contact.getDetails().getId());
        LOG_DEBUG("U:%d L:%d\n",contact.getDetails().getId(), isLoggedIn);
        contact.setOnline(isLoggedIn);
    }
}
