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

void ChatServer::onMessageReceived(libwebsocket*      websocket,
                                   const std::string& message)
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
            UserPOD userCredentials = p_jsonParser->getUser();
            login(userCredentials, websocket);
            break;
        }

        default:
        {
            break;
        }
    }
}

void ChatServer::onDisconnected(libwebsocket* websocket)
{
    std::string username;
    for (auto client = m_loggedClients.begin();
         client != m_loggedClients.end();
         ++client)
    {
        if (client->second.websocket == websocket)
        {
            username = client->first;
            std::cout << "Disconnected user: " << username << std::endl;
            m_loggedClients.erase(client);
            break;
        }
    }
}

void ChatServer::login(UserPOD user, libwebsocket* websocket)
{
    UserDAO userDao;

    if (m_loggedClients.find(user.username) != m_loggedClients.end())
    {
        std::string authFailedJson = p_jsonFactory->createLoginFailedJSON(
            AUTH_ALREADY_LOGGED_IN);
        p_websocketServer->sendMessage(websocket,authFailedJson);
        return;
    }

    if (!userDao.isValidUser(user))
    {
        std::cout << "FAILED LOGIN" << std::endl;
        std::string authFailedJson = p_jsonFactory->createLoginFailedJSON(
            AUTH_FAILED);
        p_websocketServer->sendMessage(websocket,authFailedJson);
        return;
    }

    std::cout << "Logged in user: " << user.username << std::endl;
    m_loggedClients[user.username] = UserValues {user.id, websocket};

    std::string authFailedJson = p_jsonFactory->createLoginSuccessfulJSON(user);
    p_websocketServer->sendMessage(websocket,authFailedJson);
}
