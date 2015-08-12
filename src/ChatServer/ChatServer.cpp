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

	LOG_DEBUG_METHOD;
	if (!p_jsonParser->trySetJsonString(message))
    {
        LOG_DEBUG("Received message: %s is invalid\n", message.c_str());
        return;
    }

    REQUEST_ACTION_TYPE actionType = p_jsonParser->getActionType();
    switch(actionType)
    {
        case REQUEST_LOGIN:
        {
			LoginRequestJson requestJson = p_jsonParser->tryGetLoginRequestJson();
			tryLogInUser(requestJson, hdl);
            break;
        }

        case REQUEST_GET_CONTACTS:
        {
            handleGetContactsRequest(hdl);
            break;
        }

        case REQUEST_SEND_MESSAGE:
        {
			SendMessageJson requestJson = p_jsonParser->tryGetSendMessageJson();
			handleSendMessage(requestJson, hdl);
            break;
        }

		case REQUEST_ADD_CONTACT:
		{
			handleAddContact(p_jsonParser->tryGetAddContactJson(), hdl);
			break;
		}

		case REQUEST_ADD_CONTACT_RESOLUTION:
		{
			handleAddContactResolution(p_jsonParser->tryGetAddContactResolutionJson(), hdl);
			break;
		}

		case REQUEST_REMOVE_CONTACT:
		{
			handleRemoveContact(p_jsonParser->tryGetRemoveContactJson(), hdl);
			break;
		}

        default:
        {
            break;
        }
    }
}

void ChatServer::handleAddContact(const AddContactJson& requestJson, connection_hdl hdl)
{
	const std::string& userName = requestJson.getUserName();
	//TODO: get requester's userName (first id from hdl, the userName from dao)
	std::string responseJson = p_jsonFactory->createAddingByContactJsonString("ass");
}

void ChatServer::handleAddContactResolution(const AddContactResolutionJson& requestJson, connection_hdl hdl)
{
	const std::string& userName = requestJson.getUserName();
	bool accepted = requestJson.hasAccepted();
	if (accepted)
	{
		//add in db
		//send confirmation to requester
		//send contacts list to requester and accepter
	}
	else
	{
		//send decline to requester
	}

}

void ChatServer::handleRemoveContact(const RemoveContactJson& requestJson, connection_hdl hdl)
{
	int contactId = requestJson.getContactId();
	//get remover's id from hdl
	//remove relation from dao
	//send notification to removed
}

void ChatServer::onDisconnected(connection_hdl hdl)
{
    notifyContactsOnOnlineStatusChanged(getUserId(hdl),false);

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

void ChatServer::tryLogInUser(const LoginRequestJson& requestJson,
                              connection_hdl          hdl)
{
	LOG_DEBUG_METHOD;
	const UserCredentials& userCredentials = requestJson.getUserCredentials();
	
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

    notifyContactsOnOnlineStatusChanged(userDetails.getId(),true);
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

websocketpp::connection_hdl ChatServer::getConnection(int userId)
{
    auto con = m_loggedClients.left.find(userId);
    if (con == m_loggedClients.left.end())
    {
        throw std::exception();
    }
    return con->second;
}

void ChatServer::handleGetContactsRequest(connection_hdl hdl)
{
    LOG_DEBUG_METHOD;
    int userId = getUserId(hdl);
	std::vector<Contact>& contacts = p_userDAO->getContacts(userId);

    setContactsOnlineStatus(contacts);
    std::string contactsJsonResponse =
        p_jsonFactory->createGetContactsResponseJsonString(contacts);

    p_websocketServer->sendMessage(hdl, contactsJsonResponse);
}

void ChatServer::handleSendMessage(const SendMessageJson& requestJson, websocketpp::connection_hdl hdl)
{
	Message message = requestJson.getMessage();
    int senderId = getUserId(hdl);
    if (senderId != -1)
    {
        message.setSenderId(senderId);
        std::string receiveMessageJson = p_jsonFactory->createReceiveMessageJsonString(message);
        LOG_DEBUG("R:%s", receiveMessageJson.c_str());
        try
        {
            connection_hdl peerHdl = getConnection(message.getReceiverId());

            p_websocketServer->sendMessage(peerHdl, receiveMessageJson);
        }
        catch(const std::exception&)
        {
            LOG_DEBUG("RECEIVER NOT FOUND\n");
        }
    }
}

void ChatServer::setContactsOnlineStatus(std::vector<Contact>& contacts)
{
    for(Contact& contact: contacts)
    {
        bool isLoggedIn = isUserLoggedIn(contact.getId());
        //LOG_DEBUG("U:%d L:%d\n",contact.getDetails().getId(), isLoggedIn);
		if (isLoggedIn)
		{
			contact.setState(ONLINE);
		}
    }
}

void ChatServer::notifyContactsOnOnlineStatusChanged(int userId, bool isOnline)
{
	if (userId != -1)
	{
		LOG_DEBUG("U:%d O:%d\n", userId, isOnline);

		std::string notificationMessage;
		if (isOnline)
		{
			notificationMessage = p_jsonFactory->createContactStateChangedJsonString(userId, ONLINE);
		}
		else
		{
			notificationMessage = p_jsonFactory->createContactStateChangedJsonString(userId, OFFLINE);
		}
		const std::vector<Contact>& contacts = p_userDAO->getContacts(userId);
		for (auto contact : contacts)
		{
			if (isUserLoggedIn(contact.getId()))
			{
				LOG_DEBUG("Notify:%d\n", contact.getId());
				p_websocketServer->sendMessage(getConnection(contact.getId()), notificationMessage);
			}
		}
	}

}
